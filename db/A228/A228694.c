/*
 * A228694  Number of labeled graphs on 2n nodes with degree set {1,3}, with
 *          multiple edges and loops allowed.
 * https://oeis.org/A228694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 186, a(3) = 22960, a(4) = 6831650,
 *            a(5) = 4071581010. For n > 5 the recurrence is 3*a(n) = 3*(2*n -
 *            1)*(3*n^2 - 3*n + 1)*a(n-1) + 3*(n-1)*(2*n - 3)*(2*n - 1)*(10*n
 *            + 7)*a(n-2) - 2*(n-2)*(n-1)*(2*n - 5)*(2*n - 3)*(2*n - 1)*(3*n +
 *            4)*a(n-3) + 2*(n-3)*(n-2)*(n+1)*(2*n - 7)...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utilities for dynamic memory of seq_t arrays. */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Memoisation cache. */
static seq_t *cache          = NULL;   /* array of sequence values */
static size_t cache_cap      = 0;       /* current capacity of cache   */
static idx_t   cache_len     = 0;       /* number of stored elements   */

/* Expand cache to at least indices 0..n. */
static void
ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    /* Allocate a larger table, copying old values. */
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence.  The array is filled lazily
   and cached so that repeated calls with any ordering of n are fast. */
seq_t
a(idx_t n)
{
    if (n >= cache_len) {
        ensure_capacity(n);
        for (idx_t k = cache_len; k <= n; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)1;
            } else if (k == 1) {
                cache[1] = (seq_t)5;
            } else if (k == 2) {
                cache[2] = (seq_t)186;
            } else if (k == 3) {
                cache[3] = (seq_t)22960;
            } else if (k == 4) {
                cache[4] = (seq_t)6831650;
            } else if (k == 5) {
                cache[5] = (seq_t)4071581010;
            } else {
                /* Compute terms of the recurrence. */
                idx_t K = k;   /* local copy for readability */

                /* term1 = 3*(2K-1)*(3K^2-3K+1)*a(K-1) */
                seq_t term1 = cache[K-1];
                term1 *= (seq_t)3;
                term1 *= (seq_t)(2*K - 1);
                term1 *= (seq_t)(3*K*K - 3*K + 1);

                /* term2 = 3*(K-1)*(2K-3)*(2K-1)*(10K+7)*a(K-2) */
                seq_t term2 = cache[K-2];
                term2 *= (seq_t)3;
                term2 *= (seq_t)(K - 1);
                term2 *= (seq_t)(2*K - 3);
                term2 *= (seq_t)(2*K - 1);
                term2 *= (seq_t)(10*K + 7);

                /* term3 = 2*(K-2)*(K-1)*(2K-5)*(2K-3)*(2K-1)*(3K+4)*a(K-3) */
                seq_t term3 = cache[K-3];
                term3 *= (seq_t)2;
                term3 *= (seq_t)(K - 2);
                term3 *= (seq_t)(K - 1);
                term3 *= (seq_t)(2*K - 5);
                term3 *= (seq_t)(2*K - 3);
                term3 *= (seq_t)(2*K - 1);
                term3 *= (seq_t)(3*K + 4);

                /* term4 = 2*(K-3)*(K-2)*(K+1)*(2K-7)*(2K-5)*(2K-3)*(2K-1)*a(K-4) */
                seq_t term4 = cache[K-4];
                term4 *= (seq_t)2;
                term4 *= (seq_t)(K - 3);
                term4 *= (seq_t)(K - 2);
                term4 *= (seq_t)(K + 1);
                term4 *= (seq_t)(2*K - 7);
                term4 *= (seq_t)(2*K - 5);
                term4 *= (seq_t)(2*K - 3);
                term4 *= (seq_t)(2*K - 1);

                /* Numerator = term1 + term2 + term4 - term3 */
                seq_t num = term1;
                num += term2;
                num += term4;
                num -= term3;

                cache[K] = num / (seq_t)3;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
