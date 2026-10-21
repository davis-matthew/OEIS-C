/*
 * A216851  a(n) = T^(floor(log(n)/log(2)))(n) (see comment).
 * https://oeis.org/A216851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T^(floor(log(n)/log(2)))(n) where T(x) = x/2 for even x,
 *            T(x) = (3x+1)/2 for odd x
 * Range:     u8 n <= 30, u16 n <= 2045, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 1, but safe guard */
        return (seq_t)0;

    /* compute the number of Collatz iterations = floor(log2(n)) */
    idx_t steps = 0;
    idx_t tmp = n >> 1;          /* start at second most significant bit */
    while (tmp) {
        ++steps;
        tmp >>= 1;
    }

    seq_t x = (seq_t)n;
    for (idx_t i = 0; i < steps; ++i) {
        if (x & 1)          /* odd: (3x+1)/2 */
            x = x * 3U + 1U;
        /* even or after odd adjustment: divide by 2 */
        x >>= 1;
    }
    return x;
}
