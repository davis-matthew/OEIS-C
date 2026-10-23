/*
 * A033617  Coordination sequence T2 for Zeolite Code TSC.
 * https://oeis.org/A033617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the rational generating function G(x) = (1+x)^3
 *            * (1 - x + x^2) * (1 + x^2) * (1 + x^2 + x^3 + x^4 + x^5 + 2*x^6
 *            + x^7 + 3*x^8 + x^9 + 2*x^10 + x^11 + x^12 + x^13 + x^14 + x^16)
 *            / (1 - x)^3 * (1 - x + x^2 - x^3 + x^4) * (1 + x + x^2 + x^3 +
 *            x^4) * (1 + x^3 + x^6) * (1 + x + x^2 +...
 * Range:     u8 n <= 13, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>


/* multiply two integer polynomials A[0..degA] and B[0..degB];
   result placed in out[0..degOut] (degOut = degA+degB) */
static void poly_mul(const int *A, size_t degA,
                     const int *B, size_t degB,
                     int *out, size_t degOut)
{
    for (size_t i = 0; i <= degOut; ++i) out[i] = 0;
    for (size_t i = 0; i <= degA; ++i)
        for (size_t j = 0; j <= degB; ++j)
            out[i + j] += A[i] * B[j];
}

/* compute denominator coefficients D[0..23] */
static void compute_denominator(int *D)
{
    /* factor polynomials */
    const int f1[4] = { 1, -3, 3, -1 };          /* (1 - x)^3          */
    const int f2[5] = { 1, -1, 1, -1, 1 };        /* (1 - x + x^2 - x^3 + x^4) */
    const int f3[5] = { 1, 1, 1, 1, 1 };          /* (1 + x + x^2 + x^3 + x^4) */
    const int f4[7] = { 1, 0, 0, 1, 0, 0, 1 };    /* (1 + x^3 + x^6)   */
    const int f5[7] = { 1, 1, 1, 1, 1, 1, 1 };    /* (1 + x + ... + x^6) */

    int t1[8];   /* deg 7  (3+4) */
    int t2[13];  /* deg 11 (7+4) */
    int t3[18];  /* deg 17 (11+6) */
    int t4[24];  /* deg 23 (17+6) */

    poly_mul(f1, 3, f2, 4, t1, 3 + 4);   /* deg 7  */
    poly_mul(t1, 7, f3, 4, t2, 7 + 4);   /* deg 11 */
    poly_mul(t2, 11, f4, 6, t3, 11 + 6); /* deg 17 */
    poly_mul(t3, 17, f5, 6, t4, 17 + 6); /* deg 23 */

    for (size_t i = 0; i <= 23; ++i)
        D[i] = t4[i];
}

/* compute numerator coefficients N[0..23] */
static void compute_numerator(int *N)
{
    const int f1[4] = { 1, 3, 3, 1 };   /* (1 + x)^3            */
    const int f2[3] = { 1, -1, 1 };     /* (1 - x + x^2)        */
    const int f3[3] = { 1, 0, 1 };      /* (1 + x^2)            */
    const int P[17] = {
        1, 0, 1, 1, 1, 1, 2, 1, 3, 1,
        2, 1, 1, 1, 1, 0, 1
    };                                  /* P(x) as given */

    int t1[6];   /* deg 5  (3+2) */
    int t2[8];   /* deg 7  (5+2) */
    int t3[24];  /* deg 23 (7+16) */

    poly_mul(f1, 3, f2, 2, t1, 3 + 2);   /* deg 5  */
    poly_mul(t1, 5, f3, 2, t2, 5 + 2);   /* deg 7  */
    poly_mul(t2, 7, P, 16, t3, 7 + 16); /* deg 23 */

    for (size_t i = 0; i <= 23; ++i)
        N[i] = t3[i];
}

/* sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static int denom[24];
    static int numer[24];

    if (!initialized) {
        compute_denominator(denom);
        compute_numerator(numer);

        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        initialized = true;
    }

    if (n >= cache_size) {
        if ((idx_t)cache_cap <= n) {
            size_t new_cap = cache_cap;
            while ((idx_t)new_cap <= n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            size_t maxk = (i < 24) ? (size_t)i : 23;
            for (size_t k = 1; k <= maxk; ++k) {
                seq_t term = cache[i - k];
                int coeff = denom[k];
                if (coeff < 0) {
                    sum -= term * (seq_t)(-coeff);
                } else {
                    sum += term * (seq_t)coeff;
                }
            }
            seq_t rhs = (i < 24) ? (seq_t)numer[i] : 0;
            cache[i] = rhs - sum;
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
}
