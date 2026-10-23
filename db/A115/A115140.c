/*
 * A115140  O.g.f. inverse of Catalan A000108 o.g.f.
 * https://oeis.org/A115140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 20, i64 n <= 36, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115140
   a(0) = 1
   a(n) = -Catalan(n-1)  for n>=1
   Recurrence for Catalan numbers:
   C_0 = 1
   C_k = C_{k-1} * (4k-2) / (k+1)   (k>=1)
   We compute a(n) by caching Catalan values up to n-1. */

#include <stdlib.h>
#include <stdint.h>

/* Euclidean GCD for idx_t (unsigned 64‑bit) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* a(0) is the only special case */
    if (n == 0) return (seq_t)1;

    /* We need Catalan(n-1) */
    idx_t m = n - 1;               /* index of required Catalan number */

    /* static cache: cache[k] holds Catalan(k) for k in [0, cache_len-1] */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of elements currently stored */

    /* Ensure the cache stores Catalan values up to index m */
    if (cache_len <= m) {
        /* Allocate or grow the cache */
        size_t new_len = (size_t)(m + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed, return dummy value */

        cache = tmp;

        /* If the cache was empty, initialise Catalan(0) and set length */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;   /* Catalan(0) = 1 */
            cache_len = 1;
        }

        /* Compute new entries using the recurrence C_k = C_{k-1}*(4k-2)/(k+1) */
        for (idx_t i = cache_len; i <= m; ++i) {
            idx_t k = i - 1;                 /* we are computing C_i from C_{i-1} */
            idx_t num = 4 * k + 2;            /* numerator factor (4k-2) is 4*k+2 */
            idx_t den = k + 2;                /* denominator factor (k+1) is k+2 */

            /* Reduce the fraction (num/den) to avoid overflow */
            idx_t g = gcd_idx(num, den);
            num /= g;
            den /= g;

            /* Now update the Catalan number: C_i = C_{i-1} / den * num */
            cache[i] = cache[i - 1];
            cache[i] = cache[i] / (seq_t)den;
            cache[i] *= (seq_t)num;
        }

        cache_len = new_len;                /* update stored length */
    }

    /* Return the sequence value a(n) = -Catalan(n-1) */
    return -cache[m];
}
