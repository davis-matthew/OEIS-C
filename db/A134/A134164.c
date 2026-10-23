/*
 * A134164  a(n) = 8*(6*n^2-37*n+60)*n^(n-7)*(2*n-7)!/(n-4)!.
 * https://oeis.org/A134164
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*(6*n^2-37*n+60)*n^(n-7)*(2*n-7)!/(n-4)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   bigint_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef OEIS_A134164_H
#define OEIS_A134164_H
#include <stdint.h>
#include "../utils/bigint_pow.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* undefined for n < 4 */

    seq_t nseq = n;
    /* coefficient 8*(6*n^2-37*n+60) */
    seq_t poly = (seq_t)6 * nseq * nseq - (seq_t)37 * nseq + 60;
    seq_t coef = 8 * poly;

    /* product (n-3) * (n-2) * ... * (2*n-7) */
    seq_t prod = 1;
    for (idx_t k = n - 3; k <= 2 * n - 7; ++k) {
        prod *= (seq_t)k;
    }

    seq_t result = coef * prod;

    if (n < 7) {
        /* divide by n^(7-n) to handle negative exponent */
        seq_t div = bigint_pow(nseq, 7 - n);
        result /= div;
    } else {
        /* multiply by n^(n-7) for positive exponent */
        seq_t powpart = bigint_pow(nseq, n - 7);
        result *= powpart;
    }

    return result;
}
#endif /* OEIS_A134164_H */
