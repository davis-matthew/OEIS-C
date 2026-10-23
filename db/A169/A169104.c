/*
 * A169104  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 43 * 42^(n-1) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();

    /* Compute 42^(n-1) */
    seq_t pow = seq_pow(seq_from_str("42"), n - 1);

    /* Multiply by 43 */
    return seq_mul(seq_from_str("43"), pow);
}
