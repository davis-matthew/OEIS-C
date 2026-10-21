/*
 * A074606  a(n) = 3^n + 5^n.
 * https://oeis.org/A074606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 5^n Recurrence: a(0) = 2, a(1) = 8, a(n) = 8*a(n-1)
 *            - 15*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache so that calls in arbitrary order are cheap */
    static seq_t *cache = NULL;          /* array that holds a(0)…a(k) */
    static idx_t size  = 0;              /* current size of cache (number of valid entries) */
    idx_t i;

    if (n >= size) {
        /* grow the cache to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;                    /* allocation failure – undefined result */

        /* copy previous values if any */
        for (i = 0; i < size; ++i)
            new_cache[i] = cache[i];

        /* initialise the new entries using the recurrence */
        for (i = size; i <= n; ++i) {
            if (i == 0) {
                new_cache[0] = (seq_t)2;
            } else if (i == 1) {
                new_cache[1] = (seq_t)8;
            } else {
                /* a(i) = 8*a(i-1) - 15*a(i-2) */
                seq_t t1 = new_cache[i-1] * (seq_t)8;
                seq_t t2 = new_cache[i-2] * (seq_t)15;
                new_cache[i] = t1 - t2;
            }
        }

        free(cache);
        cache = new_cache;
        size  = new_size;
    }
    return cache[n];
}
