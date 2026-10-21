/*
 * A096376  a(n) = n + (n-1)^2 + (n+1)^2.
 * https://oeis.org/A096376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^2 + n + 2
 * Range:     u8 n <= 11, u16 n <= 180, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    return 2 * nseq * nseq + nseq + (seq_t)2;
}
