/*
 * A047945  Number of 10-colorings of an n X n grid, up to rotational
 *          symmetry.
 * https://oeis.org/A047945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^(n^2) + 2*10^((n^2 + 3*(n mod 2))/4) + 10^((n^2 + (n
 *            mod 2))/2))/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t n_sq = n * n;                         /* n^2 */
    idx_t exp1 = n_sq;                          /* n^2           */
    idx_t exp2 = (n_sq + 3 * (n & 1)) >> 2;     /* (n^2 + 3*(n%2))/4 */
    idx_t exp3 = (n_sq + (n & 1)) >> 1;         /* (n^2 + (n%2))/2   */

    seq_t term1 = pow_uint(10, exp1);
    seq_t term2 = pow_uint(10, exp2);
    seq_t term3 = pow_uint(10, exp3);

    /* term2 appears twice */
    seq_t sum = term1 + term2 + term2 + term3;
    seq_t result = sum / (seq_t)4;
    return result;
}
