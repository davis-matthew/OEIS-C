/*
 * A169089  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = c(n) + 26 * Σ_{i=1}^{24} a(n-i) – 351
 *            * a(n-25) where c(n) = { 2 for 1 ≤ n ≤ 24, 1 for n = 25, 0 for n
 *            > 25, 1 for n = 0 (handled separately) } This recurrence follows
 *            from the rational generating function (t^25 + 2*t^24 + … + 2*t +
 *            1)/(351*t^25 – 26*t^24 – … – 26*t + 1...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, free */

seq_t a(idx_t n)
{
    /* Special case for the offset 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate space for a[0..n] */
    size_t len = (size_t)n + 1;
    seq_t *v = (seq_t *)calloc(len, sizeof(seq_t));
    if (!v) return (seq_t)0;   /* out of memory – should not happen */

    v[0] = (seq_t)1;              /* a(0) */

    /* sum24 holds the sum of the last up to 24 terms: a[i-1]+…+a[i-24] */
    seq_t sum24 = v[0];          /* contains a[0] for the first step */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff;
        if (i <= 24)
            coeff = (seq_t)2;
        else if (i == 25)
            coeff = (seq_t)1;
        else
            coeff = (seq_t)0;

        seq_t val = coeff + (26 * sum24);
        if (i >= 25)                /* subtract 351*a[i-25] when applicable */
            val -= (351 * v[i - 25]);

        v[i] = val;

        /* Update the sliding window for next iteration */
        sum24 += v[i];
        if (i >= 24)                 /* remove the term that leaves the window */
            sum24 -= v[i - 24];
    }

    seq_t result = v[n];
    free(v);
    return result;
}
