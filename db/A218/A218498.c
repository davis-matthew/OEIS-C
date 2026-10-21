/*
 * A218498  6th iteration of the hyperbinomial transform on the sequence of
 *          1's.
 * https://oeis.org/A218498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} 6 * (n-j+6)^(n-j-1) * C(n,j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the 6th iteration of the hyperbinomial transform on the constant 1-sequence. */

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* use multiplicative binomial coefficient to avoid repeated factorials */
    seq_t result = 0;
    seq_t six = 6;
    seq_t comb = 1;                    /* C(n,0) */

    for (idx_t i = 0; i <= n; ++i) {
        /* term for current i (where i = n-j) */
        seq_t term;

        if (i == 0) {
            /* (i+6)^(i-1) = 6^-1, 6 * 6^-1 = 1, so term = comb = 1 */
            term = comb;               /* = 1 */
        } else {
            seq_t power = big_pow((seq_t)(i + 6), (seq_t)(i - 1));
            /* term = 6 * comb * power */
            term = comb;
            term = term * six;
            term = term * power;
        }

        result += term;

        /* update comb to C(n, i+1) for next iteration */
        if (i < n) {
            int64_t top = (int64_t)(n - i);     /* n-i fits in 64‑bit */
            int64_t bottom = (int64_t)(i + 1);
            comb = comb * (seq_t)top / (seq_t)bottom;
        }
    }

    return result;
}
