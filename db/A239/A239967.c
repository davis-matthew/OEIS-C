/*
 * A239967  a(n) = a(n-1)*a(n-2) - a(n-3)*a(n-4), starting with 1,1,2,2.
 * https://oeis.org/A239967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2) - a(n-3)*a(n-4) * a(1)=1, a(2)=1, a(3)=2,
 *            a(4)=2
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 11, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the OEIS sequence A239967 */
seq_t a(idx_t n)
{
    /* a(0) is undefined for this sequence; just return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Base values for indices 1..4 */
    static const seq_t base[5] = {0, 1, 1, 2, 2};

    /* Static cache: cache[i] holds a(i), cache[0] unused */
    static seq_t *cache      = NULL;   /* allocated array */
    static size_t  cap       = 0;      /* current capacity (including index 0) */
    static idx_t   last_seen = 0;      /* largest n for which cache[n] is valid */

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed; return 0 as a fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_seen + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = base[i];
        } else {
            cache[i] = cache[i-1] * cache[i-2] - cache[i-3] * cache[i-4];
        }
    }

    if (last_seen < n)
        last_seen = n;

    return cache[n];
}
