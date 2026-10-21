/*
 * A055937  a(n) = a(n-1) * a(n-2) - 1.
 * https://oeis.org/A055937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * a(n-2) - 1, a(1) = 2, a(2) = 2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, 1‑based index */
    static seq_t *cache = NULL;
    static idx_t cache_last = 0;   /* highest index stored */

    /* allocate initial cache entries for n = 1,2 */
    if (cache_last == 0) {
        size_t init_size = 3;                     /* indices 0,1,2 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* out of memory -> 0 */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache_last = 2;
    }

    /* extend cache if needed */
    if (n > cache_last) {
        size_t new_size = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_size);
        if (!tmp) return (seq_t)0;                /* out of memory -> 0 */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * cache[i - 2] - (seq_t)1;
        }
        cache_last = n;
    }

    /* return stored value */
    return cache[n];
}
