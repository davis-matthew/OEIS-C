/*
 * A036071  Expansion of 1/(1-5*x)^5.
 * https://oeis.org/A036071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4)*5^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t bin = 1;
    for (int i = 1; i <= 4; ++i) {
        bin = bin * (((seq_t)n) + (seq_t)(5 - i)) / (seq_t)i;
    }
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    return bin * pow5;
}
