/*
 * A025853  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A025853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 - 2*n + 16*n*r + 6*n*s + (8*r + 3*s -
 *            1)^2)/576) where r = (n+2) mod 3, s = (n+3) mod 4
 * Range:     u8 n <= 1, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n) {
    idx_t r = (n + 2) % 3;
    idx_t s = (n + 3) % 4;

    seq_t nseq = (seq_t)n;
    seq_t term1 = nseq * nseq;
    seq_t term2 = (seq_t)16 * nseq * (seq_t)r;
    seq_t term3 = (seq_t)6 * nseq * (seq_t)s;

    int64_t inner = (int64_t)8 * r + (int64_t)3 * s - 1;
    seq_t inner_sq = (seq_t)(inner * inner);

    seq_t term = term1 + term2 + term3 + inner_sq;

    seq_t two_n = nseq + nseq;
    seq_t numerator = term - two_n;

    seq_t result = numerator / (seq_t)576;
    return result;
}
