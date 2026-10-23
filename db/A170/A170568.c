/*
 * A170568  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 0), a(n) = 31 * 30^(n-1) (n ≥ 1) (OEIS sequence
 *            A170568)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* Compute 30^(n-1) and multiply by 31 */
    seq_t pow = seq_pow((seq_t)30, n - 1);
    return seq_mul((seq_t)31, pow);
}
