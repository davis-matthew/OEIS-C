/*
 * A124203  a(n) = 2n + "reverse of n-written-in-binary" + 2.
 * https://oeis.org/A124203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n + reverse_of_n_in_binary + 2
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 955 (2026-11-01)
 * Helpers:   reverse_bits_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_bits_idx_to_seq.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t rev = reverse_bits_idx_to_seq(n);
    return nn * 2 + rev + 2;
}
