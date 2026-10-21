/*
 * A080572  Number of ordered pairs (i,j), 0 <= i,j < n, for which (i & j) is
 *          nonzero, where & is the bitwise AND operator.
 * https://oeis.org/A080572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(0) = a(1) = 0 a(2n) = 3·a(n) + n² a(2n+1) = a(n) + 2·a(n+1) +
 *            n² – 1
 * Range:     u8 n <= 20, u16 n <= 273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoized values */
static idx_t cache_max = 0;   /* size of cache */

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (cache_max <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                      /* allocation failed */
            return 0;
        /* Zero‑initialize the new entries */
        for (idx_t i = cache_max; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_max = new_cap;
    }

    /* Base cases */
    cache[0] = 0;
    if (n >= 1)
        cache[1] = 0;

    /* Compute values up to n recursively using the recurrence,
       iteratively filling the cache. */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0)         /* already computed */
            continue;

        idx_t k = i >> 1;          /* k = floor(i / 2) */

        if ((i & 1) == 0) {        /* even: a(2k) */
            seq_t k2 = (seq_t)k * (seq_t)k;
            cache[i] = (seq_t)3 * cache[k] + k2;
        } else {                   /* odd: a(2k+1) */
            seq_t k2 = (seq_t)k * (seq_t)k;
            seq_t tmp = cache[k] + (seq_t)2 * cache[k + 1] + k2;
            cache[i] = tmp - 1;
        }
    }

    return cache[n];
}
