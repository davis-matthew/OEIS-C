/*
 * A239530  Number of (n+1) X (1+1) 0..2 arrays with no element equal to all
 *          horizontal neighbors or unequal to all vertical neighbors, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A239530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 3*a(n-3), a(1)=1, a(2)=1, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0; return 0 for safety */
        return 0;

    static seq_t *cache = NULL;      /* stored values, indices start at 1   */
    static size_t cache_cap = 0;     /* current allocated capacity (max index) */
    static idx_t cache_last = 0;     /* largest index that has been computed */

    /* grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) return 0;  /* allocation failed, return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)1;
        else if (i == 3)
            cache[i] = (seq_t)6;
        else
            cache[i] = ((seq_t)2 * cache[i - 1]) +
                       ((seq_t)4 * cache[i - 2]) -
                       ((seq_t)3 * cache[i - 3]);
    }
    cache_last = n;
    return cache[n];
}
