/*
 * A048502  a(n) = 2^(n-1)*(9*n-16) + 9.
 * https://oeis.org/A048502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(9*n-16) + 9, with a(0)=1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    seq_t sn = (seq_t)n;
    seq_t pow = pow2(n - (idx_t)1);
    seq_t coeff = (seq_t)9 * sn - (seq_t)16;
    return pow * coeff + (seq_t)9;
}
