/*
 * A169380  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1: a(n)=31*30^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                    /* a(0) = 1 */
    /* compute 30^(n-1) as a seq_t */
    seq_t pow = pow_uint(30ULL, n - 1);
    /* multiply by 31 */
    return mul_small(pow, 31);
}
