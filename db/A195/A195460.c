/*
 * A195460  a(n) = 2^(2*n+1) - 3*2^n - 1.
 * https://oeis.org/A195460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+1) - 3*2^n - 1, n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^(2*n+1) */
    seq_t term1 = pow2(2 * n + 1);

    /* compute 2^n */
    seq_t term2 = pow2(n);

    /* subtract 3*2^n */
    term1 -= 3 * term2;

    /* subtract the final 1 */
    term1 -= (seq_t)1;

    return term1;
}
