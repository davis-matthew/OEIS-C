/*
 * A126398  Number of base 11 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n base‑11 strings where successive
 *            digits differ by ≤2. Recurrence (for n≥2): dp_next[p] =
 *            Σ_{d:|p-d|≤2} dp[d] with dp[d] = 1 for all d at n=1. Sum of dp
 *            gives a(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the a(n) value. */
seq_t a(idx_t n)
{
    /* Handle the trivial cases. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)11;

    /* DP arrays: dp[d] = number of strings of current length ending in digit d. */
    seq_t dp[11], next[11];
    int d, p, diff;

    /* Length 1: one string for each digit 0…10. */
    for (d = 0; d <= 10; ++d)
        dp[d] = (seq_t)1;

    /* Build strings up to length n. */
    for (idx_t len = 2; len <= n; ++len)
    {
        /* Initialize next array to zero. */
        for (d = 0; d <= 10; ++d)
            next[d] = (seq_t)0;

        /* Transition from dp to next. */
        for (d = 0; d <= 10; ++d)
        {
            for (diff = -2; diff <= 2; ++diff)
            {
                p = d + diff;
                if (p >= 0 && p <= 10)
                    next[p] += dp[d];
            }
        }

        /* Copy next into dp for the next iteration. */
        for (d = 0; d <= 10; ++d)
            dp[d] = next[d];
    }

    /* Sum over all ending digits to get a(n). */
    seq_t total = (seq_t)0;
    for (d = 0; d <= 10; ++d)
        total += dp[d];

    return total;
}
