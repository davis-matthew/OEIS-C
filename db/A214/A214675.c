/*
 * A214675  a(n) = 9*n^2 - 13*n + 5.
 * https://oeis.org/A214675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 13*n + 5
 * Range:     u8 n <= 6, u16 n <= 86, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)9 * nn * nn - (seq_t)13 * nn + (seq_t)5;
}
