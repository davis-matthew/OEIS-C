/*
 * A116402  Number of representations of n as a sum of distinct elements of
 *          the Fibonacci-type sequence beginning 1, 5, 6, 11, 17, ...
 * https://oeis.org/A116402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of distinct elements
 *            of the Fibonacci‑type sequence 1, 5, 6, 11, 17, … Implements a
 *            0/1 subset‑sum dynamic programming algorithm
 * Range:     u8 n <= 6438, u16 n <= 9854, u32 n <= 9854, u64 n <= 9854,
 *            u128 n <= 9854, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Generate all Fibonacci‑type terms <= n.  They are small; an array
       of fixed size is sufficient for any n reasonable for this
       contest (e.g. n <= 10^9 yields < 200 terms). */
    enum { MAX_TERMS = 128 };
    idx_t terms[MAX_TERMS];
    int term_cnt = 0;

    seq_t f0 = 1, f1 = 5;
    if ((seq_t)f0 <= (seq_t)n && term_cnt < MAX_TERMS) terms[term_cnt++] = (idx_t)f0;
    if ((seq_t)f1 <= (seq_t)n && term_cnt < MAX_TERMS) terms[term_cnt++] = (idx_t)f1;

    while (term_cnt < MAX_TERMS) {
        seq_t next = f0 + f1;
        if (next > (seq_t)n) break;
        terms[term_cnt++] = (idx_t)next;
        f0 = f1;
        f1 = next;
    }

    /* Allocate DP array: dp[s] = number of representations of s. */
    seq_t *dp = (seq_t *)malloc((idx_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failure – unlikely in tests */

    /* Initialise DP table. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* 0/1 knapsack counting: each term used at most once. */
    for (int i = 0; i < term_cnt; ++i) {
        idx_t t = terms[i];
        for (idx_t s = n; s >= t; --s) {
            dp[s] += dp[s - t];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
