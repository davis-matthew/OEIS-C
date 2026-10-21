/*
 * A235535  a(n) = binomial(9*n, 3*n) / (6*n + 1).
 * https://oeis.org/A235535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(9*n, 3*n) / (6*n + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_9n_3n(idx_t n)
{
    seq_t res = 1;            /* current result */
    idx_t N = 9 * n;          /* upper argument of binomial */
    idx_t K = 3 * n;          /* lower argument of binomial */

    /* Compute binomial coefficient using multiplicative formula
       with factor cancellation to avoid overflow for built‑in integer
       types; for arbitrary‑precision types this also works. */
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = N - K + i;   /* numerator factor */
        seq_t den = i;           /* denominator factor */

        /* Cancel common factors between num and den */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Cancel common factors between res and den */
        g = gcd(res, den);
        res /= g;
        den /= g;

        /* At this point den should be 1; multiply res by remaining num */
        res *= num;
        /* (Optional sanity check: den should be 1 for valid binomial) */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 1. Compute binomial(9*n, 3*n) safely */
    seq_t binom = binom_9n_3n(n);

    /* 2. Compute denominator 6*n + 1 */
    seq_t denom = (seq_t)n * 6 + 1;

    /* 3. Exact division – the result is an integer by definition */
    return binom / denom;
}
