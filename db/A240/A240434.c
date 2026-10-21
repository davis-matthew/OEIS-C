/*
 * A240434  Binomial transform of the sum of the first n even squares
 *          (A002492).
 * https://oeis.org/A240434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n-1) * n * (5 + 6*n + n^2)) / 3
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t nt = (seq_t)n;
    /* 2^(n-1) */
    seq_t two_pow = big_pow((seq_t)2, (seq_t)(n - 1));
    /* n * (5 + 6*n + n^2) */
    seq_t poly = nt * ((seq_t)5 + (seq_t)6 * nt + nt * nt);
    seq_t prod = two_pow * poly;
    return prod / (seq_t)3;
}
