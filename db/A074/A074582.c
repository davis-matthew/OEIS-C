/*
 * A074582  a(n) = S(3n), where S(n) is the generalized tribonacci sequence
 *          A001644.
 * https://oeis.org/A074582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) + a(n-3) with a(0)=3, a(1)=7, a(2)=39
 *            (The sequence is A074582: a(n)=S(3n) where S(n) is the
 *            tribonacci sequence A001644)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first three terms if not already done */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure (shouldn't happen in tests) */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)39;
    }

    /* if requested value is already computed, return it */
    if (n < cache_size) return cache[n];

    /* grow the cache to hold up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* compute val = 7*cache[i-1] - 5*cache[i-2] + cache[i-3] */
        seq_t val = cache[i - 1] * (seq_t)7;
        val += cache[i - 3];
        seq_t sub = cache[i - 2] * (seq_t)5;
        val -= sub;  /* guaranteed non‑negative for the sequence */
        cache[i] = val;
    }
    cache_size = new_size;
    return cache[n];
}
