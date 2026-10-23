/*
 * A022391  Fibonacci sequence beginning 1, 21.
 * https://oeis.org/A022391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2); a(0)=1, a(1)=21
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    idx_t i;

    /* ensure cache has at least entries for 0 and 1 */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)21;  /* a(1) */
    }

    /* extend cache if needed */
    if (n >= cache_cap) {
        /* allocate up to n inclusive */
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – abort */
            exit(1);
        }
        cache = new_cache;

        /* fill new entries by the recurrence */
        for (i = cache_cap; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)21;
            } else {
                cache[i] = cache[i - 1] + cache[i - 2];
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
