/*
 * A170080  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0 a(n) = 23 * 22^(n-1) for n >= 1 (Number of
 *            reduced words in the Coxeter group with 23 generators)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0ULL) {
        return (seq_t)1;
    }
    /* exponent is (n-1) */
    seq_t exp = (seq_t)(n - 1ULL);
    seq_t pow22 = big_pow((seq_t)22, exp);
    return (seq_t)23 * pow22;
}
