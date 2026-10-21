/*
 * A221568  Number of 0..3 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - a(n-3) + a(n-4) (n ≥ 5) Initial
 *            values: a(1) = 0, a(2) = 10, a(3) = 26, a(4) = 100
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, NULL */
#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoised values; index 0 left unused */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t  cached_up_to = 0;   /* highest n for which cache[n] is known */

    /* Handle very small n directly (no cache needed) */
    if (n <= 4) {
        switch ((unsigned)n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)10;
        case 3: return (seq_t)26;
        case 4: return (seq_t)100;
        }
    }

    /* Ensure cache is large enough */
    if (cache == NULL || (size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* In a real program we might abort or return an error;
               for this harness we just return 0 on allocation failure. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise first four values if not already cached */
    if (cached_up_to < 4) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)100;
        if (cached_up_to < 4) cached_up_to = 4;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 3*a(i-1) + 2*a(i-2) - a(i-3) + a(i-4) */
        seq_t val = 3 * cache[i - 1];
        val += 2 * cache[i - 2];
        val -= cache[i - 3];
        val += cache[i - 4];
        cache[i] = val;
    }

    cached_up_to = n > cached_up_to ? n : cached_up_to;
    return cache[n];
}
