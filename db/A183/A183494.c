/*
 * A183494  Half the number of nX1 0..2 arrays with no element equal to the
 *          sum mod 3 of its horizontal and vertical neighbors.
 * https://oeis.org/A183494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - 4*a(n-4) with base values a(1)=1,
 *            a(2)=3, a(3)=4, a(4)=8
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1, value for n=0 is unused */

    static seq_t *cache = NULL;   /* cache[i-1] holds a(i) */
    static idx_t cache_len = 0;   /* number of terms cached so far */
    static idx_t cache_cap = 0;   /* capacity of cache array */

    /* Allocate or grow cache as needed */
    if (cache == NULL) {
        cache_cap = n > 4 ? n : 5;          /* at least enough for base terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* memory allocation failed; caller must handle */
    } else if (cache_cap < n) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cache_cap = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 1: val = 1; break;
            case 2: val = 3; break;
            case 3: val = 4; break;
            case 4: val = 8; break;
            default:
                /* recurrence: a(n) = 2*a(n-1) + 2*a(n-3) - 4*a(n-4) */
                val = 2 * cache[i - 2] + 2 * cache[i - 4] - 4 * cache[i - 5];
                break;
        }
        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
