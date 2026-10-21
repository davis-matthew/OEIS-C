/*
 * A231104  Number of n X 4 0..3 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 6; for n ≥ 3, a(n) = 8*a(n-1) + 9*a(n-2) (OEIS
 *            A231104)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Not used, but return 0 for safety */
        return (seq_t)0;
    }

    /* Initialise cache for indices 1 and 2 */
    if (cache == NULL) {
        cache_len = 3;            /* alloc size = n+1 for n=2 */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) {
            /* Allocation failed – return 0 as sentinel */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;      /* unused */
        cache[1] = (seq_t)0;      /* a(1) */
        cache[2] = (seq_t)6;      /* a(2) */
    }

    /* Extend cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        if (new_cache == NULL) {
            /* Reallocation failed – abort computation */
            return (seq_t)0;
        }
        cache = new_cache;
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (cache[i - 1] * (seq_t)8) + (cache[i - 2] * (seq_t)9);
        }
        cache_len = new_len;
    }

    return cache[n];
}
