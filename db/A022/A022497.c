/*
 * A022497  Conjectured number of irreducible multiple zeta values of depth 9
 *          and weight 2n+25.
 * https://oeis.org/A022497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022497 (Conjectured number of irreducible multiple zeta values of depth 9 and weight 2n+25)
   Recurrence derived from the generating function:
       a(n) + Σ_{k=1}^{29} d_k · a(n-k) = N(n)   (n ≥ 0)
   where d_k are the coefficients of  
       Q(x) = (1–x)²·(1–x²)³·(1–x³)²·(1–x⁶)·(1–x⁹),
   and N(n) are the numerator coefficients (N(n)=0 for n>18).
   The first two values are  a(0)=2, a(1)=7.  The recurrence is valid for
   all n ≥ 0. */

#include <stdlib.h>
#include <stdint.h>

/* Coefficients of Q(x) = (1-x)²(1-x²)³(1-x³)²(1-x⁶)(1-x⁹) */
static const int d[30] = {
    1,   /* x⁰ */
   -2,   /* x¹ */
   -2,   /* x² */
    4,   /* x³ */
    4,   /* x⁴ */
   -2,   /* x⁵ */
  -10,   /* x⁶ */
    2,   /* x⁷ */
   11,   /* x⁸ */
   -3,   /* x⁹ */
   -6,   /* x¹⁰ */
    0,   /* x¹¹ */
    7,   /* x¹² */
   -2,   /* x¹³ */
   -8,   /* x¹⁴ */
    8,   /* x¹⁵ */
    2,   /* x¹⁶ */
   -7,   /* x¹⁷ */
    0,   /* x¹⁸ */
    6,   /* x¹⁹ */
    3,   /* x²⁰ */
   -11,  /* x²¹ */
   -2,   /* x²² */
   10,   /* x²³ */
    2,   /* x²⁴ */
   -4,   /* x²⁵ */
   -4,   /* x²⁶ */
    2,   /* x²⁷ */
    2,   /* x²⁸ */
   -1    /* x²⁹ */
};

/* Numerator coefficients of P(x) (zero for indices >18) */
static const int N[19] = {
    2,  /* x⁰ */
    3,  /* x¹ */
    6,  /* x² */
    5,  /* x³ */
    5,  /* x⁴ */
    8,  /* x⁵ */
    7,  /* x⁶ */
    3,  /* x⁷ */
    6,  /* x⁸ */
    5,  /* x⁹ */
    0,  /* x¹⁰ */
    0,  /* x¹¹ */
    3,  /* x¹² */
    0,  /* x¹³ */
   -3,  /* x¹⁴ */
   -2,  /* x¹⁵ */
    2,  /* x¹⁶ */
    2,  /* x¹⁷ */
   -1   /* x¹⁸ */
};

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;     /* allocated size */
    static size_t  next   = 0;     /* next index to compute */

    /* Ensure cache can hold indices up to n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache) abort();          /* memory allocation failed */
        cache = new_cache;
        cap   = need;
    }

    /* Compute terms from 'next' up to and including 'n' */
    for (idx_t i = next; i <= n; ++i) {
        seq_t sum = 0;

        /* Add numerator contribution, if any */
        if (i <= 18) {
            sum += (seq_t)N[(size_t)i];
        }

        /* Subtract contributions from previous terms */
        for (int k = 1; k <= 29; ++k) {
            if (i >= (idx_t)k) {
                seq_t prod = cache[i - (idx_t)k] * (seq_t)d[k];
                sum -= prod;
            }
        }

        cache[i] = sum;
    }

    next = (size_t)n + 1;
    return cache[n];
}
