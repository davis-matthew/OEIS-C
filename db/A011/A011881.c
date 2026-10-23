/*
 * A011881  a(n) = floor(n*(n-1)/28).
 * https://oeis.org/A011881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/28)
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)         /* a(0) = 0 */
        return 0;
    seq_t prod = (seq_t)n * (seq_t)(n - 1);
    return prod / 28;
}
