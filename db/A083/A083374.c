/*
 * A083374  a(n) = n^2 * (n^2 - 1)/2.
 * https://oeis.org/A083374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(n^2-1)/2 (binomial(n^2,2) for n>=1)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;        /* cast index to seq_t first */
    seq_t NN = N * N;           /* n^2 */
    return NN * (NN - 1) / 2;   /* n^2*(n^2-1)/2 */
}
