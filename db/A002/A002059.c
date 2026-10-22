/*
 * A002059  Number of partitions of an n-gon into (n-4) parts.
 * https://oeis.org/A002059
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3) * binomial(2n-6, n)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_seq.h"

seq_t a(idx_t n) {
    idx_t m = 2 * n - 6;
    seq_t binom_val = binom_seq(m, n);
    seq_t coeff = (seq_t)(n - 3);
    return coeff * binom_val;
}
