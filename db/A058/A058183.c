/*
 * A058183  Number of digits in concatenation of first n positive integers.
 * https://oeis.org/A058183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + floor(log10(n)) + 1, a(1) = 1
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9372 (2026-11-01)
 * Helpers:   num_digits_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/num_digits_idx_to_seq.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;  /* behaviour below offset is irrelevant */
    }
    seq_t total = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        total += num_digits_idx_to_seq(i);
    }
    return total;
}
