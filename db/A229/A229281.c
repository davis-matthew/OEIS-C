/*
 * A229281  Number of ascending runs in {1,...,7}^n.
 * https://oeis.org/A229281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n-1)*(4*n+3) for n>0, a(0)=0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t pow_val = big_pow((seq_t)7, (seq_t)(n - 1));
    seq_t mult_val = ((seq_t)4 * (seq_t)n) + (seq_t)3;
    return pow_val * mult_val;
}
