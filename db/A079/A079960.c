/*
 * A079960  Number of permutations satisfying -k <= p(i) - i <= r and p(i) - i
 *          not in I, i=1..n, with k=1, r=5, I={2,3}.
 * https://oeis.org/A079960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-5) + a(n-6) a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=5, a(5)=9, a(6)=16 This computes the number of
 *            compositions of n using parts {1,2,5,6}
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for computed values */
static seq_t *dp = NULL;
static size_t dp_capacity = 0;

/* ensure the DP array can hold indices up to 'needed' (inclusive) */
static void ensure_capacity(size_t needed) {
    if (dp_capacity <= needed) {
        size_t new_cap = dp_capacity ? dp_capacity * 2 : 16;
        while (new_cap <= needed) new_cap <<= 1;
        seq_t *newp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        dp = newp;
        dp_capacity = new_cap;
    }
}

seq_t a(idx_t n) {
    size_t idx = (size_t)n;
    ensure_capacity(idx);
    /* initialise the base values (once per run, harmless to overwrite) */
    dp[0] = (seq_t)1;
    if (idx >= 1) dp[1] = (seq_t)1;
    if (idx >= 2) dp[2] = (seq_t)2;
    if (idx >= 3) dp[3] = (seq_t)3;
    if (idx >= 4) dp[4] = (seq_t)5;
    if (idx >= 5) dp[5] = (seq_t)9;
    if (idx >= 6) dp[6] = (seq_t)16;

    /* compute values up to idx using the recurrence */
    for (size_t i = 7; i <= idx; ++i) {
        seq_t val = dp[i - 1];
        val += dp[i - 2];
        val += dp[i - 5];
        val += dp[i - 6];
        dp[i] = val;
    }
    return dp[idx];
}
