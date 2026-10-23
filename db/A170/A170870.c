/*
 * A170870  a(0) = 1, a(1) = 2; a(2^i + j) = a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(2^i + j) = a(j) + 3*a(j + 1) for 0 <= j < 2^i
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   greatest_pow2_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/greatest_pow2_leq.h"

/* Cache for memoisation */
static seq_t *cache      = NULL;   /* values of a(k) */
static size_t cache_cap  = 0;      /* allocated size in elements */
static idx_t  cache_len  = 0;      /* number of elements already computed */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Ensure the cache is large enough */
    size_t needed_cap = (size_t)n + 1;
    if (cache_cap < needed_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure: return 0 */
        cache     = tmp;
        cache_cap = needed_cap;
    }

    /* Initialise the first two values if not already done */
    if (cache_len < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t pw = greatest_pow2_leq(i);
        idx_t j  = i - pw;
        cache[i] = cache[j] + (seq_t)3 * cache[j + 1];
    }
    cache_len = (idx_t)n + 1;

    return cache[n];
}
