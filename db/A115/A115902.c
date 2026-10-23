/*
 * A115902  Expansion of (1-8*x)^(-3/2).
 * https://oeis.org/A115902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * (2*n + 1) * binomial(2*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t binom = 1;
    for (idx_t k = 1; k <= n; ++k) {
        binom = binom * ((seq_t)n + (seq_t)k) / (seq_t)k;
    }

    seq_t val = binom * ((((seq_t)2 * (seq_t)n) + (seq_t)1));
    val = val * pow2(n);
    return val;
}
