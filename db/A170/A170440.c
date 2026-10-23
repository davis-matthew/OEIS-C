/*
 * A170440  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, if n = 0; a(n) = 47 * 46^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small, pow_uint, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();          /* a(0) = 1 */
    /* compute 46^(n‑1) and multiply by 47 */
    return mul_small(pow_uint(46, n - 1), 47);
}
