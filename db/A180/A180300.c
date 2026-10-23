/*
 * A180300  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-10.
 * https://oeis.org/A180300
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * binomial(n+8, n-2) for n > 20 For n ≤ 20 it is
 *            computed as a(n) = f(n, n-10) – f(n, n-11) where f(r, s) counts
 *            weak compositions of r into r parts each ≤ s
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t count_compositions(idx_t n, idx_t m)
{
    /* Count weak compositions of n into n parts with each part ≤ m. */
    idx_t maxk = n / (m + 1);
    seq_t pos = 0, neg = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term = binom(n, k);
        idx_t rem = 2 * n - k * (m + 1) - 1;  /* = (n - k*(m+1)) + (n-1) */
        term = term * binom(rem, n - 1);
        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;  /* pos ≥ neg for all n, m */
}

seq_t a(idx_t n)
{
    if (n < 11)      /* undefined below offset, return 0 */
        return 0;

    if (n >= 21)     /* fast closed form for large n */
    {
        return ((seq_t)n) * binom(n + 8, n - 2);
    }

    /* Compute with the inclusion‑exclusion definition for small n. */
    seq_t f1 = count_compositions(n, n - 10);  /* max part ≤ n-10 */
    seq_t f2 = count_compositions(n, n - 11);  /* max part ≤ n-11 */
    return f1 - f2;
}
