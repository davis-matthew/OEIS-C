/*
 * A207306  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-2) + 6*a(n-3) + 4*a(n-4) + a(n-5)
 *               - 2*a(n-6) - 2*a(n-7)          for n > 9
 * Initial values (n = 1 … 9):
 *   a(1) =  6
 *   a(2) =  36
 *   a(3) =  98
 *   a(4) =  271
 *   a(5) =  665
 *   a(6) = 1675
 *   a(7) = 4344
 *   a(8) = 11081
 *   a(9) = 28136
 *
 * This implementation memoises the sequence in a static array so that
 * subsequent calls are O(1) after the term has been computed.
 */
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* 1‑based array of computed terms */
static idx_t  cached_last = 0;       /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for n < 1 */

    /* initialisation (executed once) */
    if (!cache) {
        /* capacity up to index 9 (inclusive) */
        size_t cap = 10;            /* indices 0 … 9 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure */

        /* fill the first 9 known values */
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)98;
        cache[4] = (seq_t)271;
        cache[5] = (seq_t)665;
        cache[6] = (seq_t)1675;
        cache[7] = (seq_t)4344;
        cache[8] = (seq_t)11081;
        cache[9] = (seq_t)28136;

        cached_last = 9;
    }

    /* extend the cache if necessary */
    if (n > cached_last) {
        size_t new_cap = (size_t)n + 1;   /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_last + 1; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + cache[i-2]
                     + (seq_t)6 * cache[i-3]
                     + (seq_t)4 * cache[i-4]
                     + cache[i-5]
                     - (seq_t)2 * cache[i-6]
                     - (seq_t)2 * cache[i-7];
        }
        cached_last = n;
    }

    return cache[n];
}
