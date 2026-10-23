/*
 * A154139  Indices k such that 4 plus the k-th triangular number is a perfect
 *          square.
 * https://oeis.org/A154139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A154139: a(n) = k where 4 + k(k+1)/2 is a perfect square
   Recurrence (for n > 4): a(n) = 6*a(n-2) - a(n-4) + 2
   Base values: a(1)=0, a(2)=6, a(3)=9, a(4)=39
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store computed terms, 0‑based index */
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* initialise first four terms on first call */
    if (cached == 0) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory: return 0 */
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)6;   /* a(2) */
        cache[2] = (seq_t)9;   /* a(3) */
        cache[3] = (seq_t)39;  /* a(4) */
        cached = 4;
    }

    /* extend cache up to n if needed */
    if ((size_t)n > cached) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* out of memory: return 0 */
        cache = new_cache;
        const seq_t two = (seq_t)2;
        const seq_t six = (seq_t)6;
        for (size_t i = cached; i < (size_t)n; ++i) {
            /* recurrence uses 1‑based indices:
               i is 0‑based; a(i+1) = 6*a(i-1) - a(i-3) + 2
               which corresponds to cache[i] = 6*cache[i-2] - cache[i-4] + 2
               for i >= 4
            */
            cache[i] = six * cache[i - 2] - cache[i - 4] + two;
        }
        cached = (size_t)n;
    }

    return cache[n - 1];
}
