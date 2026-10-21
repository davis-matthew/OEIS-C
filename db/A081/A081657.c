/*
 * A081657  a(n) = 2*7^n-3^n.
 * https://oeis.org/A081657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*7^n-3^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    return pow7 * (seq_t)2 - pow3;
}
