/*
 * A028050  Expansion of 1/((1-3x)(1-4x)(1-10x)(1-11x)).
 * https://oeis.org/A028050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*11^(n+3)-4*10^(n+3)+4^(n+4)-3^(n+4))/168
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>            /* malloc, realloc, exit */
#include "../utils/mul_small.h"/* mul_small(seq_t x, unsigned int m) */

static seq_t *cache = NULL;   /* memoised sequence values */
static idx_t cache_len = 0;   /* number of elements currently stored */

/* Ensure that cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* already have enough terms */
    if (cache_len > n)
        return;

    /* Determine required size */
    idx_t new_len = n + 1;
    if (new_len < 4)
        new_len = 4;

    /* Reallocate storage if needed */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp && new_len > 0) /* allocation failed */
        exit(EXIT_FAILURE);
    cache = tmp;

    /* Cache length before allocation (used for initializing base values) */
    idx_t old_len = cache_len;

    /* Initialize base values if not already present */
    if (old_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)515;
        cache[3] = (seq_t)7910;
    } else if (old_len < 4) {
        if (old_len <= 0)
            cache[0] = (seq_t)1;
        if (old_len <= 1)
            cache[1] = (seq_t)28;
        if (old_len <= 2)
            cache[2] = (seq_t)515;
        if (old_len <= 3)
            cache[3] = (seq_t)7910;
    }

    /* Compute the missing terms */
    idx_t start = (old_len > 3) ? old_len : 4;
    if (start <= n) {
        for (idx_t i = start; i <= n; ++i) {
            seq_t t1 = mul_small(cache[i - 1], 28);   /* 28 * a[n-1] */
            seq_t t2 = mul_small(cache[i - 2], 269);  /* 269 * a[n-2] */
            seq_t t3 = mul_small(cache[i - 3], 1022); /* 1022 * a[n-3] */
            seq_t t4 = mul_small(cache[i - 4], 1320); /* 1320 * a[n-4] */

            seq_t val = t1 + t3;
            val = val - t2;
            val = val - t4;
            cache[i] = val;
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
