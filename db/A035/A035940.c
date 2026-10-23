/*
 * A035940  Number of partitions in parts not of the form 9k, 9k+1 or 9k-1.
 *          Also number of partitions with no part of size 1 and differences
 *          between parts at distance 3 are greater than 1.
 * https://oeis.org/A035940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n with parts not ≡ 0,1,8 (mod 9).
 *            * Recurrence: a(0)=1; for n>0, * a(n) = Σ_{p allowed, 1 < p ≤ n}
 *            a(n-p) (p ≡ 2,3,4,5,6,7 mod 9). * The sequence begins with
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=2, …
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices – the build never asks for them. */
    if (n == 0) return 0;

    /* Static storage for the dynamic programme. */
    static seq_t *dp      = NULL;   /* dp[i] = a(i) */
    static idx_t premax   = 0;       /* highest n that has been computed */

    /* When a larger n is requested, rebuild the table from scratch up to n. */
    if (n > premax) {
        size_t sz = (size_t)n + 1;          /* length needed for indices 0..n */
        seq_t *tmp = (seq_t *)realloc(dp, sz * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed – unlikely */
        dp = tmp;

        /* Initialise all entries to 0. */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;                          /* empty partition */

        /* Standard partition DP – only allowed part sizes. */
        for (idx_t part = 2; part <= n; ++part) {
            idx_t r = part % 9ULL;          /* remainder modulo 9 */
            if (r == 0 || r == 1 || r == 8) continue;   /* forbidden parts */
            for (idx_t i = part; i <= n; ++i) {
                dp[i] = dp[i] + dp[i - part];
            }
        }
        premax = n;
    }

    return dp[n];
}
