/*
 * A188530  a(n) = 2^(2*n+1) - 5*2^(n-1) - 1.
 * https://oeis.org/A188530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+1) - 5*2^(n-1) - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^(2*n+1) */
    idx_t exp1 = n * 2 + 1;
    seq_t term1 = pow2(exp1);

    /* compute 5*2^(n-1) */
    seq_t term2 = pow2(n - 1);
    term2 = term2 * (seq_t)5;

    return term1 - term2 - (seq_t)1;
}
