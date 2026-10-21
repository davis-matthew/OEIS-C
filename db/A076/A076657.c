/*
 * A076657  a(n) = (1/24) * binomial(2n,n)*(16^n-binomial(2n,n)^2). Right side
 *          of identity involving series A005148.
 * https://oeis.org/A076657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/24) * binomial(2n,n) * (16^n - binomial(2n,n)^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   binom_2n_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A076657 sequence for any non‑negative index n. */

#include <stdint.h>
#include "../utils/binom_2n_n.h"

/* Compute 16^n by repeated multiplication. */
static seq_t pow16(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 1; i <= n; ++i) {
        r *= 16;
    }
    return r;
}

seq_t a(idx_t n) {
    /* C(2n,n) */
    seq_t c2n_n = binom_2n_n(n);

    /* 16^n */
    seq_t pow = pow16(n);

    /* c2n_n^2 */
    seq_t c2n_n_sq = c2n_n * c2n_n;

    /* 16^n - C(2n,n)^2 */
    seq_t diff = pow - c2n_n_sq;

    /* C(2n,n) * diff */
    seq_t prod = c2n_n * diff;

    /* Divide by 24 */
    seq_t result = prod / (seq_t)24;

    return result;
}
