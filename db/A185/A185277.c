/*
 * A185277  a(n) = n^9 + 9^n.
 * https://oeis.org/A185277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 + 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t base_n = (seq_t)n;
    seq_t n_pow_9 = big_pow(base_n, (seq_t)9);
    seq_t pow9_n = big_pow((seq_t)9, base_n);
    return n_pow_9 + pow9_n;
}
