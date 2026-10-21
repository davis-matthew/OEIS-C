/*
 * A079966  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={0,2}.
 * https://oeis.org/A079966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079966
   Recurrence: a(n) = a(n-2) + a(n-4) + a(n-5) + a(n-6)
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=0,
                    a(4)=2, a(5)=1, a(6)=4
   This implements the recurrence iteratively, memoising results
   so that calls may arrive in any order. */
#include <stdlib.h>

static seq_t *dp = NULL;      /* memoisation table */
static size_t cap = 0;        /* allocated capacity of dp   */
static size_t last = 0;       /* highest index whose value is known */

static void ensure_size(idx_t n)
{
    /* Initialise if this is the first call */
    if (dp == NULL) {
        cap = 7;                      /* indices 0..6 are needed initially */
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)0;
        dp[2] = (seq_t)1;
        dp[3] = (seq_t)0;
        dp[4] = (seq_t)2;
        dp[5] = (seq_t)1;
        dp[6] = (seq_t)4;
        last = 6;
    }

    /* If we already have a value for n, nothing more to do */
    if ((idx_t)last >= n)
        return;

    /* Grow the table if required, using exponential growth */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        /* Initialise the new slots to zero (not strictly necessary) */
        for (size_t i = cap; i < new_cap; ++i)
            dp[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t v = 0;
        if (i >= 2) v += dp[i - 2];
        if (i >= 4) v += dp[i - 4];
        if (i >= 5) v += dp[i - 5];
        if (i >= 6) v += dp[i - 6];
        dp[i] = v;
    }
    last = (size_t)n;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return dp[(size_t)n];
}
