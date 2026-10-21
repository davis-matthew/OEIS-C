/*
 * A235886  Number of (n+1) X (1+1) 0..3 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2+n) * (3+2^n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow_n   = pow2(n);
    /* 3 + 2^n */
    seq_t term    = 3 + pow_n;
    /* 2^(n+2) = 2^n * 4 */
    seq_t pow_n2  = pow_n * 4;
    /* final product */
    return pow_n2 * term;
}
