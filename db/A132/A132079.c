/*
 * A132079  a(n) = (5^n + 3)/2.
 * https://oeis.org/A132079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n + 3)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    return (pow5 + (seq_t)3) / (seq_t)2;
}
