/*
 * A170015  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 6 * 5^(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    /* compute 5^(n-1) in arbitrary precision */
    seq_t pow5 = seq_pow((seq_t)5, n - 1);
    /* multiply by 6 */
    return seq_mul((seq_t)6, pow5);
}
