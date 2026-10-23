/*
 * A169126  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0, otherwise 17 * 16^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   seq_from_str, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"
#include "../utils/seq_from_str.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return seq_one();

    /* 16^(n-1) */
    seq_t pow16 = seq_pow(seq_from_str("16"), n - 1);

    /* result = 17 * pow16 */
    seq_t res = seq_mul(pow16, seq_from_str("17"));

    return res;
}
