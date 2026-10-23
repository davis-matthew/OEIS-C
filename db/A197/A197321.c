/*
 * A197321  a(n) = binomial(n+10, 10)*8^n.
 * https://oeis.org/A197321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10) * 8^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = C(n+10,10) * 8^n for any n ≥ 0.  */
seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+10,10) multiplicatively */
    seq_t binom = (seq_t)1;
    for (idx_t k = 1; k <= 10; ++k) {
        binom = (binom * ((seq_t)n + k)) / (seq_t)k;
    }

    /* Compute 8^n iteratively */
    seq_t pow8 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow8 *= (seq_t)8;
    }

    return binom * pow8;
}
