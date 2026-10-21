/*
 * A052761  a(n) = 3!*n*S(n-1,3), where S denotes the Stirling numbers of
 *          second kind.
 * https://oeis.org/A052761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 2; a(n) = n * (3^(n-1) - 3 * 2^(n-1) + 3) for n
 *            ≥ 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   pow_si
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_si.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    idx_t e = n - 1;
    seq_t p2 = pow_si((seq_t)2, e);
    seq_t p3 = pow_si((seq_t)3, e);
    seq_t term = p3 - 3 * p2 + 3;
    return term * (seq_t)n;
}
