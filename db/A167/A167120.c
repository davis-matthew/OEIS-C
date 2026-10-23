/*
 * A167120  a(n) = 20*a(n-1) - 64*a(n-2) + 1 for n > 2; a(0) = 1, a(1) = 22,
 *          a(2) = 376.
 * https://oeis.org/A167120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 20*a(n-1) - 64*a(n-2) + 1,
   with initial values a(0)=1, a(1)=22, a(2)=376 */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static idx_t last_cached = (idx_t)-1;

    /* handle the first three terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)22;
    if (n == 2u) return (seq_t)376;

    /* allocate and initialise cache on first use */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)3 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)376;
        last_cached = 2;
    }

    /* if we already have the requested value, return it */
    if (n <= last_cached) {
        return cache[n];
    }

    /* grow the cache to hold the new index */
    seq_t *tmp = (seq_t *)realloc(cache,
                                  (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL) {
        /* allocation failure – abort (unlikely in test environment) */
        abort();
    }
    cache = tmp;

    /* compute all new terms up to n */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        cache[i] = ((seq_t)20) * cache[i - 1]
                 - ((seq_t)64) * cache[i - 2]
                 + (seq_t)1;
    }

    last_cached = n;
    return cache[n];
}
