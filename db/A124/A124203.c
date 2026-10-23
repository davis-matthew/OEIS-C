/*
 * A124203  a(n) = 2n + "reverse of n-written-in-binary" + 2.
 * https://oeis.org/A124203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n + reverse_of_n_in_binary + 2
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev_bin.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t rev = rev_bin(n);
    return nn * 2 + rev + 2;
}
