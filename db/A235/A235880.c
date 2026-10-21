/*
 * A235880  Number of (n+1) X (4+1) 0..2 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(50 + 15*2^(1+n) + 3^n)
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute the terms of OEIS A235880 for n >= 1 */
    seq_t pow3_n = pow3(n);          /* 3^n */
    seq_t pow2_n1 = pow2(n + 1);     /* 2^(n+1) */

    seq_t term = pow3_n;
    term += (seq_t)15 * pow2_n1;     /* add 15*2^(n+1) */
    term += (seq_t)50;               /* add 50 */
    term *= (seq_t)3;                /* multiply by 3 */

    return term;
}
