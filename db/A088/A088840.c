/*
 * A088840  Denominator of sigma(4n)/sigma(n).
 * https://oeis.org/A088840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult, frac
 * Formula:   a(n) = denominator of sigma(4*n)/sigma(n). Write n = 2^e * m
 *            with m odd. Then a(n) = { 2^(e+1) - 1, if e is even (2^(e+1) -
 *            1) / 3, if e is odd } (for e = v2(n), the exponent of 2 in n)
 * Range:     u8 n <= 255, u16 n <= 9392, u32 n <= 9392, u64 n <= 9392,
 *            u128 n <= 9392, big any n
 * Helpers:   v2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* may be optional, provided by the build */
#include "../utils/v2_idx.h"

static inline seq_t pow2_seq(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= 2;
    return res;
}

seq_t a(idx_t n) {
    /* exponent of 2 in n */
    idx_t e = v2_idx(n);

    /* compute 2^(e+1) */
    seq_t pow2 = pow2_seq(e + 1);

    /* compute the denominator */
    if ((e & 1) == 0) /* e even */
        return pow2 - 1;
    else              /* e odd */
        return (pow2 - 1) / 3;
}
