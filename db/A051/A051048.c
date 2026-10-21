/*
 * A051048  Sqrt[a(n)a(n+1)+1] of A051047.
 * https://oeis.org/A051048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051048:  Sqrt[a(n)a(n+1)+1] of A051047.
   Recurrence (for n ≥ 6):
           a(n) = 15·a(n‑1) – 15·a(n‑2) + a(n‑3)
   Initial values:
           a(1)=2, a(2)=5, a(3)=31, a(4)=449, a(5)=6271 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy memoisation cache – index 0 is unused           */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;       /* number of elements allocated (0..size-1) */

    /* grow the cache if the requested index is outside it */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;                     /* indices 0 … n  */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                                 /* allocation failed */
            cache = NULL;
            cache_size = 0;
            return 0;
        }
        cache = tmp;
        cache_size = new_size;

        /* initialise the fixed initial terms (if not already present) */
        if (new_size >= 2) cache[1] = (seq_t)2;
        if (new_size >= 3) cache[2] = (seq_t)5;
        if (new_size >= 4) cache[3] = (seq_t)31;
        if (new_size >= 5) cache[4] = (seq_t)449;
        if (new_size >= 6) cache[5] = (seq_t)6271;

        /* compute new terms using the recurrence that is valid for n≥6 */
        idx_t start = old_size;
        if (start < 6) start = 6;
        for (idx_t i = start; i < new_size; ++i) {
            cache[i] = (seq_t)15 * cache[i-1]
                     - (seq_t)15 * cache[i-2]
                     + cache[i-3];
        }
    }
    return cache[n];
}
