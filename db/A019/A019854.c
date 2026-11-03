/*
 * A019854  Expansion of 1/((1-5x)(1-6x)(1-11x)).
 * https://oeis.org/A019854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=22, a(2)=333, a(n)=22*a(n-1)-151*a(n-2)+330*a(n-3)
 *            for n>=3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 191 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms currently stored */
    static size_t cache_cap   = 0;   /* allocated capacity */

    /* initialize cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return 0;   /* allocation failed – abort */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)333;
        cache_size = 3;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* grow the cache up to n (inclusive) */
    while (cache_size <= n) {
        /* double the capacity if needed */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
            if (tmp == NULL) return 0;  /* allocation failed – abort */
            cache = tmp;
        }

        /* compute a(cache_size) using the recurrence */
        seq_t term =
            (cache[cache_size - 1] * (seq_t)22 +
             cache[cache_size - 3] * (seq_t)330) -
            cache[cache_size - 2] * (seq_t)151;

        cache[cache_size] = term;
        ++cache_size;
    }

    return cache[n];
}
