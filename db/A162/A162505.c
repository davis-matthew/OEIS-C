/*
 * A162505  G.f. is the polynomial (1-x^3) * (1-x^6) * (1-x^9) * (1-x^12) /
 *          (1-x)^4.
 * https://oeis.org/A162505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Σ_{S⊆{3,6,9,12}} (-1)^{|S|} * binom(n - sum(S) + 3, 3)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial coefficient is binom(t,3) = t·(t-1)·(t-2)/6,  applied
   only when n ≥ sum(S).                                           */

#include <stddef.h>   /* for size_t */

static const idx_t  sum_subsets[16] = {
    0,  3,  6,  9, 12,  9, 12, 15,
   15, 18, 21, 18, 21, 24, 27, 30
};

static const int    coeff[16] = {
    +1, -1, -1, -1, -1, +1, +1, +1,
    +1, +1, +1, -1, -1, -1, -1, +1
};

seq_t a(idx_t n)
{
    seq_t pos = 0;      /* sum of positive contributions   */
    seq_t neg = 0;      /* sum of negative contributions   */

    for (size_t k = 0; k < 16; ++k) {
        if (n < sum_subsets[k])          /* term not yet reached */
            continue;

        idx_t t = n - sum_subsets[k] + 3;   /* t ≥ 3 */
        seq_t bin = (seq_t)t * (t - 1) * (t - 2) / 6; /* binom(t,3) */

        if (coeff[k] > 0)
            pos += bin;
        else
            neg += bin;
    }

    return pos - neg;   /* guaranteed non‑negative                      */
}
