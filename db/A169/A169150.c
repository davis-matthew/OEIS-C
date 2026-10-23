/*
 * A169150  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169150: a(0)=1, a(n)=41*40^(n-1) for n>=1 (exponential growth) */
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }
    /* compute 40^(n-1) and multiply by 41 */
    seq_t part = pow_uint(40, n - 1);
    return seq_mul(part, (seq_t)41);
}
