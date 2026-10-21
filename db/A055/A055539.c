/*
 * A055539  Number of asymmetric types of (4,n)-hypergraphs without isolated
 *          nodes, under action of symmetric group S_4; asymmetric n-covers of
 *          an unlabeled 4-set.
 * https://oeis.org/A055539
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in
 *            (1/(1-x)^16-6/((1-x)^8*(1-x^2)^4)-3/((1-x)^4*(1-x^2)^6)+
 *            8/((1-x)^4*(1-x^3)^4)+2/((1-x)^2*(1-x^2)^3*(1-x^4)^2)+
 *            6/((1-x)^4*(1-x^2)^4*(1-x^4))-8/((1-x)^2*(1-x^4)^2*(1-x^6))-
 *            4/(1-x)^8+12/(1-x)^4/(1-x^2)^2-8/(1-x)^2/(1-x^3)^2+
 *            12/(1-x)^4-12/(1-x)^2/(1-x^2)-24/(1-x)^2+24...
 * Range:     u8 none, u16 n <= 8, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/binomial.h"   /* binomial(idx_t,idx_t) -> seq_t */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t,seq_t) -> seq_t */

/* --------------------------------------------------------------------- */
/*  Helper: copy binomial(e,t) into an int64_t value (fits for small e). */
static inline int64_t binom_int(idx_t e, idx_t t)
{
    seq_t v = binomial(e, t);
    return (int64_t)v;           /* safe for e ≤ 16               */
}

/* --------------------------------------------------------------------- */
/*  Build polynomial for (1 - x^k)^e.
 *  poly[deg] holds coefficient of x^deg, length = e*k + 1            */
static void build_factor_poly(int k, int e, int64_t *poly, int *len)
{
    int maxdeg = k * e;
    for (int i = 0; i <= maxdeg; ++i) poly[i] = 0;

    for (int t = 0; t <= e; ++t) {
        int deg = k * t;
        int64_t c = binom_int(e, t);
        if (t & 1) c = -c;
        poly[deg] = c;
    }
    *len = maxdeg + 1;
}

/* --------------------------------------------------------------------- */
/*  Convolution of A[0..lenA-1] and B[0..lenB-1], result into C.
 *  The result degree never exceeds MAX_D (<=54).                */
#define MAX_D 54
static void poly_mul(const int64_t *A, int lenA,
                     const int64_t *B, int lenB,
                     int64_t *C)
{
    for (int i = 0; i <= MAX_D; ++i) C[i] = 0;

    for (int i = 0; i < lenA; ++i) {
        if (A[i] == 0) continue;
        for (int j = 0; j < lenB; ++j) {
            if (B[j] == 0) continue;
            int deg = i + j;
            if (deg <= MAX_D)
                C[deg] += A[i] * B[j];
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Term descriptor for the 14 summands of the generating function.     */
struct term_t {
    int coeff;   /* integer coefficient (may be negative)      */
    int exp[5];  /* exponents for (1-x)^16, (1-x^2)^6,      *
                   * (1-x^3)^4, (1-x^4)^2, (1-x^6)^1          */
};

static const int MAX_FACTORS = 5;
static const int FACTOR_K[5]      = {1, 2, 3, 4, 6};
static const int FACTOR_MAXE[5]   = {16, 6, 4, 2, 1};

static const struct term_t TERMS[14] = {
    {  1, {16, 0, 0, 0, 0} },   /* 1/(1-x)^16                              */
    { -6, { 8, 4, 0, 0, 0} },   /* -6/(1-x)^8 (1-x^2)^4                    */
    { -3, { 4, 6, 0, 0, 0} },   /* -3/(1-x)^4 (1-x^2)^6                    */
    {  8, { 4, 0, 4, 0, 0} },   /*  8/(1-x)^4 (1-x^3)^4                    */
    {  2, { 2, 3, 0, 2, 0} },   /*  2/(1-x)^2 (1-x^2)^3 (1-x^4)^2          */
    {  6, { 4, 4, 0, 1, 0} },   /*  6/(1-x)^4 (1-x^2)^4 (1-x^4)            */
    { -8, { 2, 0, 0, 2, 1} },   /* -8/(1-x)^2 (1-x^4)^2 (1-x^6)            */
    { -4, { 8, 0, 0, 0, 0} },   /* -4/(1-x)^8                              */
    { 12, { 4, 2, 0, 0, 0} },   /* 12/(1-x)^4 (1-x^2)^2                    */
    { -8, { 2, 0, 2, 0, 0} },   /* -8/(1-x)^2 (1-x^3)^2                    */
    { 12, { 4, 0, 0, 0, 0} },   /* 12/(1-x)^4                              */
    { -12,{ 2, 1, 0, 0, 0} },   /* -12/(1-x)^2 (1-x^2)                     */
    { -24,{ 2, 0, 0, 0, 0} },   /* -24/(1-x)^2                             */
    {  24,{ 1, 0, 0, 0, 0} }    /* 24/(1-x)                                */
};

/* --------------------------------------------------------------------- */
/*  Global tables for the recurrence.                                  */
static int64_t Q_coeff[MAX_D + 1];   /* denominator (without 24 factor)      */
static int64_t R_coeff[MAX_D + 1];   /* numerator divided by 24               */
static bool coeffs_init = false;

/* --------------------------------------------------------------------- */
/*  Build Q_coeff and R_coeff once.                                      */
static void build_coefficients(void)
{
    if (coeffs_init) return;
    coeffs_init = true;

    /* Build Q(x) = (1-x)^16 (1-x^2)^6 (1-x^3)^4 (1-x^4)^2 (1-x^6)   */
    int64_t Q[55] = {0};
    int64_t temp[55] = {0};
    int64_t poly[55];
    int lenPoly;

    Q[0] = 1;
    int degQ = 0;

    for (int f = 0; f < MAX_FACTORS; ++f) {
        build_factor_poly(FACTOR_K[f], FACTOR_MAXE[f], poly, &lenPoly);
        poly_mul(Q, degQ + 1, poly, lenPoly, temp);
        memcpy(Q, temp, (degQ + lenPoly + 1) * sizeof(int64_t));
        degQ += lenPoly - 1;
    }

    memcpy(Q_coeff, Q, (MAX_D + 1) * sizeof(int64_t));

    /* Build numerator polynomial before dividing by 24.                  */
    int64_t Numer[55];
    memset(Numer, 0, sizeof(Numer));

    for (int ti = 0; ti < 14; ++ti) {
        const struct term_t *t = &TERMS[ti];

        /* Remaining exponents after cancelling this term from Q.       */
        int rem_exp[5];
        for (int k = 0; k < MAX_FACTORS; ++k)
            rem_exp[k] = FACTOR_MAXE[k] - t->exp[k];

        /* Compute polynomial for the remaining factors.                  */
        int64_t Ptmp[55] = {0};
        int64_t poly2[55];
        int lenPoly2;
        Ptmp[0] = 1;
        int degP = 0;

        for (int k = 0; k < MAX_FACTORS; ++k) {
            int e = rem_exp[k];
            if (e == 0) continue;
            build_factor_poly(FACTOR_K[k], e, poly2, &lenPoly2);
            poly_mul(Ptmp, degP + 1, poly2, lenPoly2, temp);
            memcpy(Ptmp, temp, (degP + lenPoly2 + 1) * sizeof(int64_t));
            degP += lenPoly2 - 1;
        }

        int64_t mult = (int64_t)t->coeff;
        for (int n = 0; n <= MAX_D; ++n)
            Numer[n] += mult * Ptmp[n];
    }

    /* Divide by 24 to obtain R_coeff.                               */
    for (int n = 0; n <= MAX_D; ++n)
        R_coeff[n] = Numer[n] / 24;
}

/* --------------------------------------------------------------------- */
/*  Cache for computed values.                                           */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* --------------------------------------------------------------------- */
/*  Ensure cache is large enough and compute values up to new_cap.      */
static void ensure_cache(idx_t new_cap)
{
    if (new_cap < cache_cap) return;  /* already large enough */

    seq_t *new_cache = (seq_t*)calloc((size_t)(new_cap + 1), sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = (size_t)new_cap;

    /* Compute values up to new_cap.                                    */
    build_coefficients();

    for (size_t n = 0; n <= new_cap; ++n) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Contribution of R_coeff[n] */
        if (R_coeff[n] >= 0)
            pos += (seq_t)R_coeff[n];
        else
            neg += (seq_t)(-R_coeff[n]);

        size_t upper = (n <= MAX_D) ? n : MAX_D;
        for (size_t k = 1; k <= upper; ++k) {
            int64_t qk = Q_coeff[k];
            if (qk == 0) continue;
            seq_t prod = seq_mul(cache[(size_t)(n - k)], (seq_t)(qk > 0 ? qk : -qk));
            if (qk > 0)
                neg += prod;
            else
                pos += prod;
        }

        /* pos >= neg for all n; if not, set to zero.                      */
        if (pos < neg)
            cache[(size_t)n] = (seq_t)0;
        else
            cache[(size_t)n] = pos - neg;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure cache covers requested index */
    if (n >= cache_cap)
        ensure_cache(n);
    return cache[(size_t)n];
}
