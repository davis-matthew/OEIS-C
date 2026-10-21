/*
 * A094817  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 8 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 3, s(2n) = 3.
 * https://oeis.org/A094817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3), n ≥ 4 Initial values:
 *            a(1) = 2, a(2) = 6, a(3) = 19
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[0] is a dummy to simplify indexing      */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;            /* current number of entries in cache */

    /* Initialise cache once with the first three terms          */
    if (cache_len == 0) {
        cache_len = 4;                     /* indices 0..3 are valid           */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failure (unlikely)   */
        cache[0] = (seq_t)0;               /* unused dummy                    */
        cache[1] = (seq_t)2;               /* a(1)                            */
        cache[2] = (seq_t)6;               /* a(2)                            */
        cache[3] = (seq_t)19;              /* a(3)                            */
    }

    if (n < 1) return (seq_t)0;            /* behaviour below offset not needed */

    /* Grow cache until it contains a(n)                        */
    while (cache_len <= n) {
        idx_t i = cache_len;                /* next index to compute           */
        /* extend the array                                        */
        seq_t *tmp = (seq_t*)realloc(cache, (i + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure              */
        cache = tmp;
        /* compute via recurrence (already initialised for i=1..3) */
        seq_t val = cache[i-1] * (seq_t)6
                  - cache[i-2] * (seq_t)10
                  + cache[i-3] * (seq_t)4;
        cache[i] = val;
        cache_len = i + 1;
    }

    return cache[n];
}
