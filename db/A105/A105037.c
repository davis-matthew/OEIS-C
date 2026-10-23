/*
 * A105037  a(n) = 22*a(n-2) - a(n-4) + 10, for n > 3, with a(0) = 0, a(1) =
 *          4, a(2) = 6, a(3) = 98.
 * https://oeis.org/A105037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 22*a(n-2) - a(n-4) + 10   (n > 3) with a(0)=0, a(1)=4, a(2)=6, a(3)=98
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t cache_size = 0;

    /* initialise cache with first four terms on the first call */
    if (cache == NULL) {
        cache_capacity = 4;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;   /* a(0) */
        cache[1] = (seq_t)4;   /* a(1) */
        cache[2] = (seq_t)6;   /* a(2) */
        cache[3] = (seq_t)98;  /* a(3) */
        cache_size = 4;
    }

    /* extend capacity if necessary */
    if (n + 1 > cache_capacity) {
        idx_t new_capacity = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_capacity = new_capacity;
    }

    /* compute any missing terms using the recurrence */
    for (idx_t k = cache_size; k <= n; ++k) {
        if (k > 3) {
            cache[k] = (seq_t)22 * cache[k - 2] - cache[k - 4] + (seq_t)10;
        }
    }

    cache_size = n + 1;
    return cache[n];
}
