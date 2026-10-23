/*
 * A038865  a(n) = (n+3)^3 - n^3.
 * https://oeis.org/A038865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)^3 - n^3 = 9*(n^2 + 3*n + 3)
 * Range:     u8 n <= 3, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)9 * (nn * nn + (seq_t)3 * nn + (seq_t)3);
}
