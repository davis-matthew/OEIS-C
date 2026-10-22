/*
 * A001447  a(n) = (5^n + 5^floor(n/2))/2.
 * https://oeis.org/A001447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n + 5^floor(n/2))/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t power_n = a000248_pow((seq_t)5, n);
    seq_t power_half_n = a000248_pow((seq_t)5, n / 2);
    return (power_n + power_half_n) / 2;
}
