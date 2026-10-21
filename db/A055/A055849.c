/*
 * A055849  a(n) = 3*a(n-1) - a(n-2) with a(0)=1, a(1)=9.
 * https://oeis.org/A055849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* cached values a(0..last) */
    static idx_t last = 1;           /* index of the highest cached value */
    static size_t cap = 0;           /* allocated capacity of cache */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cap = 4;                     /* start with a small capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        last = 1;
    }

    /* Return cached value if already computed */
    if (n <= last) return cache[n];

    /* Grow cache until we have a(n) computed */
    while (n > last) {
        /* Ensure enough space */
        if (last + 1 >= cap) {
            cap <<= 1;               /* double the capacity */
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* Compute next term using the recurrence */
        seq_t val = cache[last] * (seq_t)3 - cache[last - 1];
        cache[++last] = val;
    }

    return cache[n];
}
