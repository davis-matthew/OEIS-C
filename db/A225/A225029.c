/*
 * A225029  Non-crossing, non-nesting, 3-colored set partitions.
 * https://oeis.org/A225029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 59*a(n-2) + 74*a(n-3) - a(n-4), * with
 *            a(0)=1, a(1)=4, a(2)=19, a(3)=103
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* pointer to the cache array          */
    static size_t allocated = 0;     /* size of the allocated array         */
    static idx_t last_computed = 3;  /* index of the highest computed term */

    /* initialise cache if it does not yet exist */
    if (cache == NULL) {
        allocated = 4;
        cache = (seq_t *)calloc(allocated, sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed */
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 19;
        cache[3] = 103;
    }

    /* if the requested term is already computed, just return it */
    if (n <= last_computed) return cache[n];

    /* grow the cache if necessary */
    if (allocated <= (size_t)n) {
        size_t new_alloc = allocated ? allocated * 2 : 4;
        if (new_alloc <= (size_t)n) {
            while (new_alloc <= (size_t)n)
                new_alloc <<= 1;     /* double until we can hold n */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failed */
        cache = tmp;
        allocated = new_alloc;
    }

    /* compute new terms up to n */
    idx_t i;
    for (i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(n) = 14*a(n-1) - 59*a(n-2) + 74*a(n-3) - a(n-4) */
        seq_t term1 = cache[i - 1] * 14;
        seq_t term2 = cache[i - 3] * 74;
        seq_t term3 = cache[i - 2] * 59;
        seq_t term4 = cache[i - 4];
        cache[i] = term1 + term2 - term3 - term4;
    }
    last_computed = n;

    return cache[n];
}
