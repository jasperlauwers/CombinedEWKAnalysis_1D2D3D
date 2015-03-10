/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "HiggsAnalysis/CombinedLimit/interface/RooACProcessScaling_1D.h" 

#include <math.h> 
#include "TMath.h" 
#include "RooFormulaVar.h"
#include "RooRealVar.h"
#include "RooFit.h"

#include "TFile.h"

ClassImp(RooACProcessScaling_1D) 

RooACProcessScaling_1D::RooACProcessScaling_1D() : 
  type_(notype),P_par1_histo(0),P_par1_TF(0)
{
  initializeProfiles();
}

RooACProcessScaling_1D::RooACProcessScaling_1D(const char *name, 
					     const char *title,
					     RooAbsReal& _x,
					     RooAbsReal& _par1,
					     RooAbsReal& _SM_shape,
								 const char * parFilename,LimitType typeInit) :
   RooAbsReal(name,title),
   par1("par1","par1",this,_par1),
   type_(typeInit),
   profileFilename(parFilename),
   P_par1_histo(0),
   P_par1_TF(0)
{ 
  initializeProfiles();
  initializeNormalization(_x,_SM_shape);
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(parFilename,"READ");  
  gDirectory->cd(pwd);
  std::vector<double> bins_;

  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(_x);
  int N_bins=rdep.getBinning("").numBins();

  for (int i=0;i<N_bins;i++){
    if (i==0) bins_.push_back(rdep.getBinning("").binLow(i));
    bins_.push_back(rdep.getBinning("").binHigh(i));
  }
  readProfiles(bins_,*f,type_);
  f->Close();
} 

RooACProcessScaling_1D::
RooACProcessScaling_1D(const RooACProcessScaling_1D& other, 
		      const char* name) :  
  RooAbsReal(other,name),
  par1("par1",this,other.par1),
  type_(other.type_),
  SM_integral(other.SM_integral),
  integral_basis(other.integral_basis),
  bins(other.bins),
  profileFilename(other.profileFilename),
  P_par1_histo(0),
  P_par1_TF(0)
{ 
  initializeProfiles();
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(profileFilename,"READ");  
  gDirectory->cd(pwd);  
  readProfiles(bins,*f,type_);
  f->Close();
} 

void RooACProcessScaling_1D::initializeProfiles() {
  int const N_bins=20;

  P_par1_histo = new TH1D*[N_bins]();
  P_par1_TF = new TF1*[N_bins]();
}

void RooACProcessScaling_1D::initializeNormalization(const RooAbsReal& dep,
						    const RooAbsReal& shape) {
  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(dep);
  RooRealVar& b = const_cast<RooRealVar&>(rdep);
  
  bool bin_true=rdep.hasBinning("");
  if (!bin_true)
    cout <<"\t @@@@@@@@@@@ can't find biining! (RooATGCProcessScaling file)" << endl;

  int N_bins=rdep.getBinning("").numBins();

  
  RooAbsReal* integral = shape.createIntegral(RooArgSet(dep),RooArgSet());
  SM_integral = integral->getVal();
  delete integral;

  for(int i=0; i<N_bins; ++i) {
    int bin_low=rdep.getBinning("").binLow(i);
    int bin_high=rdep.getBinning("").binHigh(i);
    const string intRange="integRange";
    b.setRange((const char*)intRange.c_str(),bin_low,bin_high); 
    RooAbsReal* integral = shape.createIntegral(RooArgSet(rdep),RooArgSet(),(const char*)intRange.c_str());
    integral_basis.push_back(integral->getVal());
    if (i==0){
      bins.push_back(bin_low);
    }
    bins.push_back(bin_high);

    delete integral;

  }



}

void RooACProcessScaling_1D::readProfiles(std::vector<double> bins,TDirectory& dir,LimitType type_) const {

  int N_bins;
  int i;
  
  switch(type_) {
  case par1_TH1:
  case par1_TF1:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }
  

  for(i=0; i<N_bins; ++i) {
    
    if (P_par1_histo[i]) delete P_par1_histo[i];
    if (P_par1_TF[i]) delete P_par1_TF[i];
    TString par2name = TString::Format("bin_content_par1_%i",i+1);

    switch(type_) {
    case par1_TH1:
      P_par1_histo[i] = dynamic_cast<TH1D *>(dir.Get(par2name)->Clone(par2name+"new"));
      P_par1_histo[i]->SetDirectory(0);
      break;
    case par1_TF1:
      P_par1_TF[i] = dynamic_cast<TF1 *>(dir.Get(par2name)->Clone(par2name+"new"));
      //      P_par1_TF[i]->SetDirectory(0);
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }
  
  }

}

void RooACProcessScaling_1D::readProfiles(RooACProcessScaling_1D const& other) {
  int N_bins;
  switch(type_) {
  case par1_TH1:
  case par1_TF1:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }


  for (int i = 0; i<N_bins; ++i) {    
    TString par2name = TString::Format("bin_content_par1_%i",i+1);

    switch(type_) {
    case par1_TH1:
      P_par1_histo[i] = dynamic_cast<TH1D *>(other.P_par1_histo[i]->Clone(par2name+"new"));
      P_par1_histo[i]->SetDirectory(0);
      break;
    case par1_TF1:
      P_par1_TF[i] = dynamic_cast<TF1 *>(other.P_par1_histo[i]->Clone(par2name+"new"));
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }

  }
}

RooACProcessScaling_1D::~RooACProcessScaling_1D() {
  int N_bins=20;

  for(int i = 0; i<N_bins; ++i) {
    if (P_par1_histo[i])
      delete P_par1_histo[i];
    if (P_par1_TF[i])
      delete P_par1_TF[i];
  }
  delete[] P_par1_histo;
  delete[] P_par1_TF;
}

Double_t RooACProcessScaling_1D::evaluate() const 
{ 
  

  TH1D ** P_histo = NULL;
  TF1 ** P_TF = NULL;
  double v1(0.0);
  
    P_histo = P_par1_histo;
    P_TF = P_par1_TF;
    v1 = par1;

  if (not P_histo[0] and not P_TF[0]) {
    TFile f(profileFilename);
    readProfiles(bins,f,type_);
    f.Close();
  }

  switch(type_) {
  case par1_TH1:
    if (v1 < P_histo[0]->GetXaxis()->GetXmin())
      v1 = P_histo[0]->GetXaxis()->GetXmin();
    if (v1 > P_histo[0]->GetXaxis()->GetXmax())
      v1 = P_histo[0]->GetXaxis()->GetXmax();
    break;
  case par1_TF1:
    if (v1 < P_TF[0]->GetXmin())
      v1 = P_TF[0]->GetXmin();
    if (v1 > P_TF[0]->GetXmax())
      v1 = P_TF[0]->GetXmax();
    break;
  case notype:
    assert(NULL && "invalid limit type!");
    break;

  }

  
  double ret(0.);
  int N_bins=bins.size()-1;;


  for(int i = 0; i<N_bins; i++) {
    switch(type_) {
    case par1_TH1:
      ret += P_histo[i]->Interpolate(v1)*integral_basis[i];
      break;
    case par1_TF1:
      ret += P_TF[i]->Eval(v1)*integral_basis[i];
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }
  }
  cout << "ret/SM_integral= "<< ret<<"/"<<SM_integral<<" = "<< ret/SM_integral << endl;
  return ret/SM_integral;

}

