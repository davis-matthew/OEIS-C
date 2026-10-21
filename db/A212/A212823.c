/*
 * A212823  Number of 0..2 arrays of length n with no adjacent pair equal to
 *          its immediately preceding adjacent pair, and new values introduced
 *          in 0..2 order.
 * https://oeis.org/A212823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) for n>5 Initial values: a(1)=1,
 *            a(2)=2, a(3)=5, a(4)=12, a(5)=33
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* previously computed values   */
static idx_t cache_size = 0;      /* number of allocated slots    */
static idx_t cache_valid = 0;     /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset 0 is unused */

    /* Ensure the cache is large enough for indices 0..n */
    if (cache == NULL || cache_size <= n) {
        size_t new_alloc = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)malloc(new_alloc * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure – should not happen */

        /* copy existing data */
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i)
                tmp[i] = cache[i];
            free(cache);
        }

        cache = tmp;
        cache_size = new_alloc;
    }

    /* Compute all values up to n */
    if (n > cache_valid) {
        for (idx_t i = cache_valid + 1; i <= n; ++i) {
            if (i == 1)
                cache[1] = 1;
            else if (i == 2)
                cache[2] = 2;
            else if (i == 3)
                cache[3] = 5;
            else if (i == 4)
                cache[4] = 12;
            else if (i == 5)
                cache[5] = 33;
            else {
                seq_t t = cache[i - 1];
                t = t + t;                /* 2 * a(i-1)   */
                seq_t u = cache[i - 2];
                u = u + u;                /* 2 * a(i-2)   */
                cache[i] = t + u;
            }
        }
        cache_valid = n;
    }

    return cache[n];
}
