/*
 * A152422  Decimal expansion of (sqrt(3)-1)/2.
 * https://oeis.org/A152422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(((sqrt(3)-1)/2) * 10^(n+1)) mod 10 (decimal digit)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define BASE        1000000000U
#define MAX_WORDS    2500          /* >20000 decimal digits in base 1e9 */
#define MAX_DIGITS 20000          /* enough for all requested n (≤10000) */

typedef struct { size_t len; uint32_t d[MAX_WORDS]; } BigInt;

/* ---------- Big integer helpers ---------- */
static void bi_set_zero(BigInt *a) { a->len = 0; }
static void bi_set_small(BigInt *a, uint32_t v)
{ if (v==0) { a->len=0; } else { a->len=1; a->d[0]=v; } }
static void bi_copy(BigInt *dst, const BigInt *src)
{ dst->len = src->len; for(size_t i=0;i<src->len;i++) dst->d[i]=src->d[i]; }
static void bi_trim(BigInt *a)
{ while(a->len && a->d[a->len-1]==0) a->len--; }
static void bi_mul_small(BigInt *a, uint32_t m)
{ if(m==0 || a->len==0){a->len=0;return;}
  uint64_t carry=0;
  for(size_t i=0;i<a->len;i++){
    uint64_t prod=(uint64_t)a->d[i]*m+carry;
    a->d[i]=(uint32_t)(prod%BASE);
    carry=prod/BASE;
  }
  while(carry){
    a->d[a->len++]=(uint32_t)carry;
    carry/=BASE;
  }
}
static void bi_add_small(BigInt *a, uint32_t v)
{ if(v==0)return;
  if(a->len==0){a->d[0]=v;a->len=1;return;}
  uint64_t sum=(uint64_t)a->d[0]+v;
  a->d[0]=(uint32_t)(sum%BASE);
  uint64_t carry=sum/BASE;
  size_t i=1;
  while(carry && i<a->len){
    uint64_t s=(uint64_t)a->d[i]+carry;
    a->d[i]=(uint32_t)(s%BASE);
    carry=s/BASE;
    i++;
  }
  if(carry){ a->d[a->len++]=(uint32_t)carry; }
}
static int bi_cmp(const BigInt *a, const BigInt *b)
{ if(a->len!=b->len) return (a->len>b->len)?1:-1;
  for(int i=(int)a->len-1;i>=0;i--)
    if(a->d[i]!=b->d[i]) return (a->d[i]>b->d[i])?1:-1;
  return 0;
}
static void bi_sub(BigInt *a, const BigInt *b)  /* assumes a >= b */
{ uint64_t borrow=0;
  for(size_t i=0;i<a->len;i++){
    uint64_t ai=a->d[i];
    uint64_t bi=(i<b->len)?b->d[i]:0;
    long long diff=(long long)ai-(long long)bi-(long long)borrow;
    if(diff<0){ diff+=BASE; borrow=1; } else borrow=0;
    a->d[i]=(uint32_t)diff;
  }
  bi_trim(a);
}

/* ---------- digits of sqrt(75) ---------- */
static uint8_t sqrt75_digits[MAX_DIGITS+1];
static int sqrt75_ready = 0;

static void compute_sqrt75(void)
{
  if(sqrt75_ready) return;

  BigInt rem, div, cand, temp, prod;
  bi_set_zero(&rem);
  bi_set_zero(&div);

  for(size_t i=0;i<=MAX_DIGITS;i++) {
    uint32_t group = (i==0)?75:0;          /* first pair 75, rest 00 */
    bi_mul_small(&rem,100);
    bi_add_small(&rem,group);

    /* candidate = div * 20 */
    bi_copy(&cand,&div);
    bi_mul_small(&cand,20);

    int d;
    for(d=9; d>=0; --d) {
      bi_copy(&temp,&cand);
      bi_add_small(&temp,(uint32_t)d);
      bi_copy(&prod,&temp);
      bi_mul_small(&prod,(uint32_t)d);
      if(bi_cmp(&prod,&rem)<=0) break;
    }
    sqrt75_digits[i]=(uint8_t)d;
    bi_sub(&rem,&prod);
    bi_mul_small(&div,10);
    bi_add_small(&div,(uint32_t)d);
  }
  sqrt75_ready = 1;
}

/* ---------- sequence entry point ---------- */
seq_t a(idx_t n)
{
  compute_sqrt75();
  if(n==0) return (seq_t)(sqrt75_digits[0]-5);   /* 8-5 = 3  */
  return (seq_t)sqrt75_digits[(size_t)n];
}
