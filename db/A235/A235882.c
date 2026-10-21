/*
 * A235882  Number of (n+1) X (6+1) 0..2 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(602 + 63*2^(1+n) + 3^n)
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t term2 = pow2(n + 1);   /* 2^(n+1) */
    seq_t term3 = pow3(n);       /* 3^n   */
    seq_t sum   = (seq_t)602 + (seq_t)63 * term2 + term3;
    return (seq_t)3 * sum;
}
