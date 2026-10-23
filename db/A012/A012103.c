/*
 * A012103  cos(arcsin(sinh(x))) =
 *          1-1/2!*x^2-7/4!*x^4-121/6!*x^6-5167/8!*x^8...
 * https://oeis.org/A012103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(1/2)*( 2^(2n-1) + Σ_{k=1}^{n-1} binom(2n,2k) * a(k) *
 *            a(n-k) ) a(0) = 1 This recurrence follows from the identity
 *            cos(arcsin(sinh(x))) = sqrt(1 - sinh(x)^2) when expanded in even
 *            powers of x
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   seq_pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure that the required utility for powering 2 is available. */
#include "../utils/seq_pow_uint.h"
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* a(0) .. a(len-1)          */
static size_t cache_len = 0;  /* number of computed entries */
static size_t cache_cap = 0;  /* allocated capacity          */

static void ensure_cache(idx_t n)
{
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            delete [] cache;
        }
#else
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if ((idx_t)(cache_len) <= n) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t c = 1;                     /* binom(2i,0) */
            for (idx_t j = 1; j <= i - 1; ++j) {
                /* Update c to binom(2i,2j) from binom(2i,2j-2)   */
                unsigned long long num1 = 2ULL * (unsigned long long)i - (2ULL * j - 2ULL);
                unsigned long long num2 = 2ULL * (unsigned long long)i - (2ULL * j - 2ULL) - 1ULL;
                unsigned long long den1 = 2ULL * j;
                unsigned long long den2 = 2ULL * j - 1ULL;

                c *= (seq_t)num1;
                c *= (seq_t)num2;
                c /= (seq_t)den1;
                c /= (seq_t)den2;

                seq_t term = c * cache[j];
                term *= cache[i - j];
                sum += term;
            }
            seq_t pow2_i = seq_pow_uint(2, 2 * i - 1);
            seq_t total = pow2_i + sum;
            seq_t a_val = -(total / (seq_t)2);
            cache[i] = a_val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
