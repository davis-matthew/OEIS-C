/*
 * A165130  a(n) = (2^(n+4)-1)*(2^n-1).
 * https://oeis.org/A165130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+4)-1)*(2^n-1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t two_n = pow2(n);

    /* Compute 2^(n+4) as 2^n * 16 */
    seq_t two_n_plus4 = two_n * (seq_t)16;

    /* Compute (2^(n+4)-1)*(2^n-1) */
    seq_t term1 = two_n_plus4 - (seq_t)1;
    seq_t term2 = two_n - (seq_t)1;
    return term1 * term2;
}
