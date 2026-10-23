/*
 * A022033  Expansion of 1/((1-x)(1-5x)(1-6x)(1-7x)).
 * https://oeis.org/A022033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A022033: Expansion of 1/((1-x)(1-5x)(1-6x)(1-7x))
   Recurrence (n≥4): a(n) = 19*a(n-1) - 125*a(n-2) + 317*a(n-3) - 210*a(n-4)
   Initial values: a(0)=1, a(1)=19, a(2)=236, a(3)=2426
   The implementation memoizes computed values in a static array.
   Multiplication by small constants is implemented as repeated addition
   to avoid overflow in the intermediate computations of the chosen
   seq_t type. */

#include <stdlib.h>
#include <stdint.h>

/* Multiply a by k by repeated addition (k <= 317).  The result fits
   in seq_t because we only use it when the final value fits in seq_t. */
static seq_t mul_small(seq_t a, unsigned int k)
{
    seq_t r = 0;
    while (k--) r += a;
    return r;
}

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* number of slots already allocated */

    /* enlarge the cache if required */
    if (n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)(n + 1);

        /* reallocate storage for seq_t objects */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (cache == NULL) abort();   /* allocation failure */

        /* compute terms that were not present before */
        for (idx_t i = old_cap; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 19;
            } else if (i == 2) {
                cache[i] = 236;
            } else if (i == 3) {
                cache[i] = 2426;
            } else {
                seq_t t1 = mul_small(cache[i - 1], 19);
                seq_t t2 = mul_small(cache[i - 2], 125);
                seq_t t3 = mul_small(cache[i - 3], 317);
                seq_t t4 = mul_small(cache[i - 4], 210);
                cache[i] = t1 - t2 + t3 - t4;
            }
        }

        cache_cap = new_cap;
    }

    return cache[n];
}
