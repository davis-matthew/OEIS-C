/*
 * A006043  A traffic light problem: expansion of 2/(1 - 3*x)^3.
 * https://oeis.org/A006043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*(n+1)*3^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t power_of_3 = a000272_ipow((seq_t)3, n);
    return (n_seq + 2) * (n_seq + 1) * power_of_3;
}
