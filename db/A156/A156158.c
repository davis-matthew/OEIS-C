/*
 * A156158  a(n) = 6*a(n-1) - a(n-2) for n > 2; a(1) = 25, a(2) = 137.
 * https://oeis.org/A156158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1)=25, a(2)=137
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of allocated slots (including index 0) */
    static idx_t cache_size = 0;  /* highest index that has been computed */

    /* Base cases */
    if (n == 1) return 25;
    if (n == 2) return 137;

    /* Allocate or grow the cache if necessary */
    if (!cache) {
        /* First allocation: at least 3 slots (index 0 unused) */
        cache_cap = (idx_t)(n + 1 > 3 ? n + 1 : 3);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* memory allocation failed, return 0 */
        cache[1] = 25;
        cache[2] = 137;
        cache_size = 2;
    } else if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* memory allocation failed, return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill cache up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1] - cache[i - 2];
    }
    cache_size = n;

    return cache[n];
}
