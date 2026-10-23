/*
 * A169277  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(0) = 1, a(1) = 24, a(2) = 552,
 *            …, a(n) = 22 * (a(n-1)+a(n-2)+…+a(n-28)) – 253 * a(n-29) + p(n)
 *            , where p(n) = 1 for n=0, 2 for 1 ≤ n ≤ 28, 1 for n=29, and 0
 *            otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Grow the cache to at least n+1 elements */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size * 2 + 16)
        new_size = cache_size * 2 + 16;   /* exponential growth for efficiency */

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed; terminate to avoid undefined behaviour */
        abort();
    }
    cache = new_cache;

    /* initialize new entries to 0 */
    memset(cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));

    /* compute missing terms */
    for (idx_t i = cache_size; i <= (idx_t)new_size - 1; ++i) {
        /* compute sum of last 28 terms */
        seq_t sum28 = 0;
        for (idx_t j = 1; j <= 28 && j <= i; ++j)
            sum28 += cache[i - j];

        /* term from a(n-29) if applicable */
        seq_t term29 = (i >= 29) ? cache[i - 29] : (seq_t)0;

        /* RHS from the numerator polynomial P(t) */
        seq_t rhs;
        if (i == 0)
            rhs = (seq_t)1;
        else if (i <= 28)
            rhs = (seq_t)2;
        else if (i == 29)
            rhs = (seq_t)1;
        else
            rhs = (seq_t)0;

        /* recurrence: a[i] = 22*sum28 - 253*term29 + rhs */
        cache[i] = (seq_t)22 * sum28 - (seq_t)253 * term29 + rhs;
    }

    /* update cache size */
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
