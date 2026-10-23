/*
 * A121110  Trajectory of 3778888999 under "x -> product of digits of x" map.
 * https://oeis.org/A121110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(1)=3778888999, a(n)=digit_product(a(n‑1)) for n>1
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Static cache for already computed terms.
       The trajectory becomes 0 after a few steps, so the
       cache never needs to grow beyond a few dozen elements. */
    static seq_t cache[64];
    static size_t cache_len = 0;

    /* Initialise the first term on first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)3778888999;
        cache_len = 1;
    }

    /* Extend the cache only until the requested index.                                   *
       If we encounter a 0 term, all following terms are also 0, so we can return       *
       immediately without extending the array.                                         */
    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        if (prev == (seq_t)0)        /* all remaining terms are 0 */
            return (seq_t)0;

        seq_t next = digit_product(prev);
        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n - 1];
}
