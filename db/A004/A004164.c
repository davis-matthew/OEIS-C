/*
 * A004164  Sum of digits of n^3.
 * https://oeis.org/A004164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of digits of n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2430, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_digits.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n3 = n_seq * n_seq * n_seq;
    return sum_digits(n3);
}
