/*
 * A048512  a(n) = T(7,n), array T given by A048505.
 * https://oeis.org/A048512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+29*n+196)*2^(n-2) - 48
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)65;

    seq_t ns = (seq_t)n;
    seq_t poly = ns * ns + (seq_t)29 * ns + (seq_t)196;
    seq_t pw   = pow2(n - 2);
    return poly * pw - (seq_t)48;
}
