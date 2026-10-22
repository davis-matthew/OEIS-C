/*
 * A005567  Number of walks on square lattice.
 * https://oeis.org/A005567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 26 + 11*n + n^2 + (-16 + 24*n + 8*n^2)*2^n
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 20, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t poly = -16 + 24*n_seq + 8*n_seq*n_seq;
    seq_t power_of_2 = pw2(n);
    return 26 + 11*n_seq + n_seq*n_seq + poly * power_of_2;
}
