/*
 * A174964  Determinant of the symmetric n X n matrix M_n where M_n(j,k) =
 *          n^2+1 for j = k, M_n(j,k) = n for abs(j-k) = 1, M_n(j,k) = 0
 *          otherwise.
 * https://oeis.org/A174964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 - n^(2*n+2))/(1 - n^2) for n > 1, with a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;

    seq_t nn   = (seq_t)n;
    seq_t n_sq = nn * nn;

    /* exponent: 2*n + 2 */
    seq_t exp = nn * (seq_t)2 + (seq_t)2;

    /* numerator: n^(2*n+2) - 1 */
    seq_t num = big_pow(nn, exp);
    num -= (seq_t)1;

    /* denominator: n^2 - 1 */
    seq_t denom = n_sq - (seq_t)1;

    return num / denom;
}
