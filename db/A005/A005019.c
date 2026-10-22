/*
 * A005019  The number of n X n (0,1)-matrices with a 1-width of 1.
 * https://oeis.org/A005019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n^2) - (2^n - 1)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   a005019_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005019_pw.h"

seq_t a(idx_t n) {
    seq_t two = 2;
    seq_t pow2n = a005019_pw(two, n);
    seq_t term1 = a005019_pw(pow2n, n);
    seq_t term2 = a005019_pw(pow2n - 1, n);
    return term1 - term2;
}
