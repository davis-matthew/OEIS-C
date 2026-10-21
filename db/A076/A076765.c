/*
 * A076765  Partial sums of Chebyshev sequence S(n,8) = U(n,4) = A001090(n+1).
 * https://oeis.org/A076765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, easy, nonn
 * Formula:   * a(0) = 1 * a(1) = 9 * a(n) = 8*a(n-1) - a(n-2) + 1 for n >= 2
 *            * (equivalently a(n+3) = 9*a(n+2) - 9*a(n+1) + a(n))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;

    /* Initialise cache for first three values */
    if (cache_size == 0) {
        cache_capacity = 4;                        /* small initial capacity */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;                /* out of memory → 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)72;                       /* follows from recurrence */
        cache_size = 3;
    }

    /* Extend cache until it holds index n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Grow capacity if needed */
        if (cache_size >= cache_capacity) {
            idx_t new_cap = cache_capacity << 1;   /* double */
            seq_t *new_ptr = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
            if (!new_ptr) return (seq_t)0;         /* out of memory */
            cache = new_ptr;
            cache_capacity = new_cap;
        }

        /* Compute a(i) using the two‑term recurrence */
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)9;
        } else {
            /* a(i) = 8*a(i-1) - a(i-2) + 1 */
            seq_t v = 8 * cache[i - 1] - cache[i - 2] + (seq_t)1;
            cache[i] = v;
        }
        ++cache_size;
    }

    /* Return the requested value */
    return cache[n];
}
