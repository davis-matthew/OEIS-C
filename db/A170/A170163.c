/*
 * A170163  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=10*9^(n-1) for n>=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small, pow_uint, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();
    /* Compute 9^(n-1) and then multiply by 10. */
    seq_t res = pow_uint(9, n - 1);
    res = mul_small(res, 10);
    return res;
}
