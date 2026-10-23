/*
 * A136780  Number of primitive multiplex juggling sequences of length n, base
 *          state <1,1,1> and hand capacity 2.
 * https://oeis.org/A136780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) – 8*a(n-2) – 7*a(n-3), a(1)=1, a(2)=2, a(3)=13
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out-of-order calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first four entries (index 0 unused) */
    if (cache == NULL) {
        cache_size = 4;                        /* indices 0 .. 3 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failure */
        cache[0] = 0;                          /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)13;
    }

    /* if n already computed, return it */
    if (n < cache_size) return cache[n];

    /* grow cache to accommodate n */
    idx_t new_size = n + 1;   /* we need indices 0 .. n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) return 0;           /* allocation failure */
    cache = tmp;

    /* compute new entries using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = ((seq_t)7) * cache[i-1]
                 - ((seq_t)8) * cache[i-2]
                 - ((seq_t)7) * cache[i-3];
    }

    cache_size = new_size;
    return cache[n];
}
