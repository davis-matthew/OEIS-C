/*
 * A169469  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 24 * 23^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
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
    if (n == 0) {
        return seq_one();          /* a(0) = 1 */
    }
    /* Compute 23^(n-1) */
    seq_t pow23 = seq_pow(seq_from_str("23"), n - 1);
    /* Multiply by 24 to get the term */
    seq_t result = seq_mul(pow23, seq_from_str("24"));
    return result;
}
