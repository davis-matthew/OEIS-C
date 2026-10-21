/*
 * A227638  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having determinant equal to one,
 *          with rows and columns of the latter in nondecreasing lexicographic
 *          order.
 * https://oeis.org/A227638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 42, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A227638 – Number of n×3 0,1 arrays …
 *
 *  Formula (generating function):  
 *      a(n) = Σ_{k=0}^{12} coeff_gf[k] * binom(n - k + 8, 9)   (n ≥ 1)
 *
 *  For n > 13 the sequence satisfies the 10‑term recurrence:
 *      a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4)
 *            + 252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8)
 *            + 10*a(n-9) - a(n-10)
 *
 *  Initial values (required for the recurrence) are obtained from the
 *  generating‑function formula for n = 1 … 13.
 *
 *  The implementation keeps a memoised cache of already‑computed terms
 *  so that calls may arrive in any order.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;          /* cache[i] == a(i+1) (1‑based index) */
static idx_t  cache_len = 0;         /* number of stored elements */

/* coefficients of the generating function numerator  (length 13) */
static const int coeff_gf[13] = {
    3,  -19,  64,  -128,  172,  -167,
    151, -154, 151,  -107,  49,  -13,   2
};

/* recurrence coefficients for n > 13  (length 10) */
static const int coeff_rec[10] = {
    10, -45, 120, -210, 252, -210, 120, -45, 10, -1
};

/* Evaluate the generating‑function formula for a given n (1‑based). */
static seq_t term_using_gf(idx_t n)
{
    seq_t result = 0;
    int k;
    for (k = 0; k < 13; ++k) {
        /* Compute num = n - k + 8 ; do it via signed arithmetic to avoid
         * unsigned underflow when n is small. */
        int64_t num = (int64_t)n + 8 - k;
        if (num < 9) continue;                    /* binom = 0 */
        seq_t bin = binom((idx_t)num, 9);
        if (bin == 0) continue;
        int c = coeff_gf[k];
        if (c >= 0)
            result += (seq_t)c * bin;
        else
            result -= (seq_t)(-c) * bin;
    }
    return result;
}

/* Ensure the cache contains terms up to index n (1‑based). */
static void ensure_cache_upto(idx_t n)
{
    if (n <= cache_len) return;

    /* Reallocate cache to hold n elements (1‑based). */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) abort();    /* out of memory – cannot continue */

    cache = new_cache;

    idx_t i;
    for (i = cache_len; i < n; ++i) {
        idx_t idx = i + 1;   /* 1‑based sequence index */

        seq_t val;
        if (idx <= 13) {
            val = term_using_gf(idx);
        } else {
            /* recurrence using the 10 immediately preceding terms */
            seq_t pos_sum = 0, neg_sum = 0;
            int j;
            for (j = 0; j < 10; ++j) {
                int diff = coeff_rec[j];
                seq_t term = cache[i - j - 1];   /* a(idx-1-j) */
                if (diff >= 0)
                    pos_sum += (seq_t)diff * term;
                else
                    neg_sum += (seq_t)(-diff) * term;
            }
            val = pos_sum - neg_sum;
        }
        cache[i] = val;
    }
    cache_len = n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */
    ensure_cache_upto(n);
    return cache[n - 1];
}
