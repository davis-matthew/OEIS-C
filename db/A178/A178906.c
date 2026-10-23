/*
 * A178906  a(n) = 111*a(n-1) - 100*a(n-2) with a(0)=0 and a(1)=1.
 * https://oeis.org/A178906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 111*a(n-1) - 100*a(n-2) with a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t max_size = 0;
    static size_t count    = 0;

    /* initialise cache on first call */
    if (!cache) {
        max_size = 2;
        cache = (seq_t*)malloc(max_size * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory – unlikely in tests */
        cache[0] = 0;
        cache[1] = 1;
        count   = 2;
    }

    /* grow cache if necessary (never shrink) */
    if ((size_t)n + 1 > max_size) {
        size_t new_cap = max_size * 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory */
        cache    = tmp;
        max_size = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = count; i <= n; ++i) {
        cache[i] = (seq_t)(111 * cache[i-1] - 100 * cache[i-2]);
    }
    if ((size_t)n + 1 > count) count = (size_t)n + 1;

    return cache[n];
}
