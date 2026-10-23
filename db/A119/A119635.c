/*
 * A119635  a(n) = n*(1 + n^2)*2^n.
 * https://oeis.org/A119635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1 + n^2)*2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t square_plus_one = n_seq * n_seq + (seq_t)1;

    seq_t result = pow2(n);
    result *= n_seq;
    result *= square_plus_one;

    return result;
}
