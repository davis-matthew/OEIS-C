/*
 * A149197  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 0,
 *          0), (1, -1, 1), (1, 0, -1), (1, 1, 0)}.
 * https://oeis.org/A149197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) is given by a linear recurrence of order k (k ≤ 64). The
 *            recurrence coefficients are obtained by Berlekamp–Massey on the
 *            first few terms of the sequence (mod 2^61‑1). The function
 *            returns exact values; for small n it uses a hand‑rolled DP; for
 *            larger n it applies the recurrence by binary exp...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   compute_exact_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/compute_exact_base.h"

/* ----- Modulo arithmetic for Berlekamp–Massey (Mersenne prime 2^61‑1) ----- */
static const uint64_t MOD = 1152921504606846975ULL;        /* 2^61-1 */

static inline uint64_t mod_add(uint64_t a, uint64_t b) { uint64_t r=a+b; if(r>=MOD) r-=MOD; return r; }
static inline uint64_t mod_sub(uint64_t a, uint64_t b) { return a>=b? a-b : a+MOD-b; }
static inline uint64_t mod_mul(uint64_t a, uint64_t b) { return (unsigned __int128)a*b%MOD; }

static uint64_t mod_pow(uint64_t a, uint64_t e) {
    uint64_t r=1;
    while(e){
        if(e&1) r=mod_mul(r,a);
        a=mod_mul(a,a);
        e>>=1;
    }
    return r;
}
static uint64_t mod_inv(uint64_t a) { return mod_pow(a,MOD-2); }

/* ----- Berlekamp–Massey ----- */
static void berlekamp_massey(const uint64_t *s, size_t len, uint64_t *rec, size_t *k) {
    uint64_t C[128] = {0}, B[128] = {0};
    size_t C_len=1, B_len=1;
    C[0]=1; B[0]=1;
    uint64_t b=1;
    size_t L=0, m=1;
    for(size_t n=0; n<len; ++n){
        uint64_t d=s[n];
        for(size_t i=1;i<=L;i++) if(C[i]) d=mod_sub(d,mod_mul(C[i],s[n-i]));
        if(d==0){ ++m; continue; }
        uint64_t coef=mod_mul(d,mod_inv(b));
        if(C_len<B_len+m){
            memset(C+C_len,0,(B_len+m-C_len)*sizeof(uint64_t));
            C_len=B_len+m;
        }
        for(size_t i=0;i<B_len;i++){
            uint64_t t=mod_mul(coef,B[i]);
            C[i+m]=mod_sub(C[i+m],t);
        }
        if(2*L<=n){
            memcpy(B,C,(C_len)*sizeof(uint64_t));
            B_len=C_len;
            b=d;
            L=n+1-L;
            m=1;
        }else++m;
    }
    *k=L;
    for(size_t i=1;i<=L;i++){
        uint64_t v=C[i];
        rec[i-1]= v? (MOD-v)%MOD : 0;
    }
}

/* ----- Compute first N terms modulo MOD ----- */
static void compute_mod_terms(uint64_t *out, size_t N) {
    size_t maxsz = N*N*N;
    uint64_t *dp_cur = (uint64_t*)calloc(maxsz, sizeof(uint64_t));
    uint64_t *dp_next = (uint64_t*)calloc(maxsz, sizeof(uint64_t));
    dp_cur[0] = 1;
    out[0] = 1;
    for(size_t step=1; step<N; ++step){
        size_t dprev = step;
        size_t dcur  = step+1;
        size_t szcur = dcur*dcur*dcur;
        memset(dp_next,0,sizeof(uint64_t)*maxsz);
        for(size_t x=0;x<dprev;x++)
        for(size_t y=0;y<dprev;y++)
        for(size_t z=0;z<dprev;z++){
            size_t idx = (x*dprev + y)*dprev + z;
            uint64_t v = dp_cur[idx];
            if(!v) continue;
            /* 1 (-1,-1,0) */
            if(x>=1 && y>=1){
                size_t nidx = ((x-1)*dcur + (y-1))*dcur + z;
                dp_next[nidx] = mod_add(dp_next[nidx],v);
            }
            /* 2 (-1,0,0) */
            if(x>=1){
                size_t nidx = ((x-1)*dcur + y)*dcur + z;
                dp_next[nidx] = mod_add(dp_next[nidx],v);
            }
            /* 3 (1,-1,1) */
            if(y>=1){
                size_t nx=x+1, ny=y-1, nz=z+1;
                if(nx<dcur && nz<dcur){
                    size_t nidx = (nx*dcur + ny)*dcur + nz;
                    dp_next[nidx] = mod_add(dp_next[nidx],v);
                }
            }
            /* 4 (1,0,-1) */
            if(z>=1){
                size_t nx=x+1, ny=y, nz=z-1;
                if(nx<dcur){
                    size_t nidx = (nx*dcur + ny)*dcur + nz;
                    dp_next[nidx] = mod_add(dp_next[nidx],v);
                }
            }
            /* 5 (1,1,0) */
            {
                size_t nx=x+1, ny=y+1, nz=z;
                if(nx<dcur && ny<dcur){
                    size_t nidx = (nx*dcur + ny)*dcur + nz;
                    dp_next[nidx] = mod_add(dp_next[nidx],v);
                }
            }
        }
        uint64_t sum=0;
        for(size_t i=0;i<szcur;i++) sum=mod_add(sum,dp_next[i]);
        out[step]=sum;
        /* swap dp_cur and dp_next */
        uint64_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }
    free(dp_cur); free(dp_next);
}

/* ----- Global state for recurrence ----- */
#define MAX_K 64
static bool initialized = false;
static size_t rec_len = 0;
static seq_t rec_coef[MAX_K];
static seq_t base_vals[MAX_K];

static void init_sequence(void) {
    const size_t N = 120;          /* number of terms used for BM (≥ 2*MAX_K) */
    uint64_t mod_seq[N];
    compute_mod_terms(mod_seq, N);
    uint64_t rec_mod[128];
    berlekamp_massey(mod_seq, N, rec_mod, &rec_len);
    if(rec_len>MAX_K) rec_len=MAX_K;  /* safety, should not happen */
    for(size_t i=0;i<rec_len;i++){
        rec_coef[i] = (seq_t)rec_mod[i];
    }
    compute_exact_base(base_vals, rec_len);
    initialized = true;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n) {
    if(!initialized) init_sequence();
    if(n < rec_len) return base_vals[n];

    size_t k = rec_len;
    /* companion matrix T and temporary storage */
    seq_t T[MAX_K][MAX_K];
    seq_t T2[MAX_K][MAX_K];
    seq_t vec[MAX_K], tmp_vec[MAX_K];

    /* build T */
    memset(T,0,sizeof(T));
    for(size_t j=0;j<k;j++) T[0][j] = rec_coef[j];
    for(size_t i=1;i<k;i++) T[i][i-1] = 1;

    /* initial vector v = [a(k-1), a(k-2), ..., a(0)] */
    for(size_t i=0;i<k;i++) vec[i] = base_vals[k-1-i];

    idx_t e = n - (k-1);
    while(e){
        if(e & 1){
            /* tmp_vec = T * vec */
            for(size_t i=0;i<k;i++){
                seq_t sum = 0;
                for(size_t j=0;j<k;j++) sum = sum + T[i][j] * vec[j];
                tmp_vec[i] = sum;
            }
            memcpy(vec,tmp_vec,sizeof(seq_t)*k);
        }
        /* T = T * T */
        memset(T2,0,sizeof(T2));
        for(size_t i=0;i<k;i++)
        for(size_t j=0;j<k;j++){
            seq_t sum = 0;
            for(size_t t=0;t<k;t++) sum = sum + T[i][t] * T[t][j];
            T2[i][j] = sum;
        }
        memcpy(T,T2,sizeof(T));
        e >>= 1;
    }
    return vec[0];
}
