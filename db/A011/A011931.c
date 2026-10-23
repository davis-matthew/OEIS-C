/*
 * A011931  a(n) = floor(n*(n-1)*(n-2)*(n-3)/21).
 * https://oeis.org/A011931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/21)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 4 the product contains a zero, so the result is 0. */
    if (n < 4)
        return 0;

    /* Cast to seq_t before any arithmetic to avoid overflow in idx_t. */
    seq_t x = (seq_t)n;
    seq_t prod = x * (x - 1) * (x - 2) * (x - 3);
    return prod / 21;
}
