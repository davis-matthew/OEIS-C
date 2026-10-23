/*
 * A167574  The fourth row of the ED3 array A167572.
 * https://oeis.org/A167574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 56*n^3 + 28*n^2 + 98*n - 15
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 424, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    return 56 * n3 + 28 * n2 + 98 * tn - 15;
}
