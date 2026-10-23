/*
 * A176662  a(0)=2, a(1)=7, and a(n) = (3*n+1)*2^(n-1) if n > 1.
 * https://oeis.org/A176662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 7, a(n) = (3*n+1)*2^(n-1) for n > 1
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 2;
    if (n == 1) return 7;
    seq_t coeff = ((seq_t)3 * (seq_t)n) + 1;
    seq_t pow = pow2(n - 1);  /* 2^(n-1) */
    return coeff * pow;
}
