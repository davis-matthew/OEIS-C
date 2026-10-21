/*
 * A094188  Number of levels in the compositions of n with odd summands.
 * https://oeis.org/A094188
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 3*a(n-4) - a(n-5) with
 *            initial values (offset 2): a(2)=1, a(3)=0, a(4)=3, a(5)=2,
 *            a(6)=8
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized dynamic programming for A094188.  The cache stores a(i)
   for i from 0 up to the largest index requested so far.  Indices
   below the offset (n<2) are defined to be 0, as required by the
   problem statement. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* values below the offset are irrelevant */

    /* static cache variables */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with the first 7 entries (indices 0..6). */
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */

        cache[0] = 0;  cache[1] = 0;          /* unused, but defined */
        cache[2] = 1;  cache[3] = 0;          /* a(2) and a(3) */
        cache[4] = 3;  cache[5] = 2;          /* a(4) and a(5) */
        cache[6] = 8;                          /* a(6) */
    }

    /* Expand cache if the requested index is beyond the current size. */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */

        /* Compute missing terms using the recurrence. */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 2 || i == 3 || i == 4 || i == 5 || i == 6)
                continue;                    /* already initialised */

            /* recurrence: a(i) = a(i-1) + 3*a(i-2) - a(i-3) - 3*a(i-4) - a(i-5) */
            seq_t sum1 = cache[i-1] + (cache[i-2] * 3);
            seq_t sum2 = cache[i-3] + (cache[i-4] * 3) + cache[i-5];
            cache[i] = sum1 - sum2;          /* guaranteed non‑negative */
        }
    }

    return cache[n];
}
