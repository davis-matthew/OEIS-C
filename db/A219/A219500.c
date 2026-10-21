/*
 * A219500  Number of n X 6 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 n X 6 array.
 * https://oeis.org/A219500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^8 c[k] * binom(n - k + 4, 5) where c = [6, -25,
 *            59, -77, 46, 0, -10, 1, 1] (All arithmetic is exact when
 *            performed in seq_t.)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binom, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* coefficients of the generating‑function numerator */
    static const int c[9] = { 6, -25, 59, -77, 46, 0, -10, 1, 1 };

    seq_t pos = 0;   /* sum of non‑negative terms */
    seq_t neg = 0;   /* sum of positive contributions from negative coefficients */

    for (int k = 0; k < 9; ++k) {
        /* term contributes only when n ≥ k+1 (so that the binomial top ≥ 5) */
        if (n < (idx_t)(k + 1))
            continue;

        seq_t B = binom(n - k + 4, 5);               /* binom(n - k + 4, 5)   */
        int    coeff = c[k];

        if (coeff >= 0) {
            /* use mul_small to avoid overflow when multiplying by coeff */
            seq_t prod = mul_small(B, (unsigned int)coeff);
            pos += prod;
        } else {
            seq_t prod = mul_small(B, (unsigned int)(-coeff));
            neg += prod;
        }
    }

    /* pos ≥ neg for all n ≥ 1, so subtraction is well‑defined */
    return pos - neg;
}
