/*
 * A176594  a(n) = 5^(2^n).
 * https://oeis.org/A176594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(2^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated size of cache   */
    static idx_t computed_max = 0; /* largest n for which cache[n] is valid */

    /* Allocate initial cache for n==0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();                /* in case of memory failure */
        cache[0] = (seq_t)5;                /* a(0) = 5                     */
        cap = 1;
        computed_max = 0;
    }

    /* If requested n exceeds current allocation, grow the cache */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            /* a(i) = a(i-1)^2  (recurrence from the OEIS entry) */
            cache[i] = cache[i - 1] * cache[i - 1];
        }
        computed_max = n;
    }

    return cache[n];
}
