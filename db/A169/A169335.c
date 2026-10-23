/*
 * A169335  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34 * 33^(n-1) for n > 0, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();          /* a(0) = 1 */

    seq_t pow = seq_one();          /* start with 33^0 = 1 */
    /* compute 33^(n-1) by repeated multiplication   */
    for (idx_t i = 0; i < n - 1; ++i)
        pow = mul_small(pow, 33);

    /* multiply by 34 to get the final result          */
    return mul_small(pow, 34);
}
