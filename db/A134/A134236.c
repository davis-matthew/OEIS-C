/*
 * A134236  Ternary numbers with equal numbers of 0's, 1's and 2's and such
 *          that read left-to-right at all times the number of 2's >= number
 *          of 1's >= number of 0's.
 * https://oeis.org/A134236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A134236: generates the n‑th ternary number that contains an equal
   number of 0’s, 1’s, and 2’s and for every prefix satisfies
   #2 ≥ #1 ≥ #0.  The digits are written as a decimal integer. */

#include <stdint.h>
#include <stdlib.h>

/* Recursive DP that counts completions from state (r0,r1,r2).
   k is the total count of each digit in the whole string. */
static idx_t count_dp(int r0, int r1, int r2, int k, idx_t *dp) {
    int stride = k + 1;
    int idx = r0 * stride * stride + r1 * stride + r2;
    if (dp[idx] != (idx_t)-1) return dp[idx];
    if (r0 == 0 && r1 == 0 && r2 == 0) return dp[idx] = 1;

    int used0 = k - r0;
    int used1 = k - r1;
    int used2 = k - r2;
    idx_t sum = 0;

    if (r0 > 0) {
        int n0 = used0 + 1, n1 = used1, n2 = used2;
        if (n2 >= n1 && n1 >= n0)
            sum += count_dp(r0 - 1, r1, r2, k, dp);
    }
    if (r1 > 0) {
        int n0 = used0, n1 = used1 + 1, n2 = used2;
        if (n2 >= n1 && n1 >= n0)
            sum += count_dp(r0, r1 - 1, r2, k, dp);
    }
    if (r2 > 0) {
        int n0 = used0, n1 = used1, n2 = used2 + 1;
        if (n2 >= n1 && n1 >= n0)
            sum += count_dp(r0, r1, r2 - 1, k, dp);
    }
    return dp[idx] = sum;
}

/* a(n) returns the n-th term of sequence A134236 (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined, but safe return */

    /* --- find the block (k) that contains the n‑th term --- */
    idx_t target = n;
    idx_t cumulative = 0;
    int k = 1;
    while (1) {
        int sz = k + 1;
        int total_states = sz * sz * sz;
        idx_t *dp = (idx_t*)malloc(total_states * sizeof(idx_t));
        if (!dp) return 0;
        for (int i = 0; i < total_states; ++i) dp[i] = (idx_t)-1;
        idx_t total_k = count_dp(k, k, k, k, dp);
        free(dp);
        if (cumulative + total_k >= target) break;
        cumulative += total_k;
        ++k;
    }

    /* --- build the (target‑cumulative)-th string of length 3k --- */
    int sz = k + 1;
    int total_states = sz * sz * sz;
    idx_t *dp = (idx_t*)malloc(total_states * sizeof(idx_t));
    if (!dp) return 0;
    for (int i = 0; i < total_states; ++i) dp[i] = (idx_t)-1;

    idx_t rank = target - cumulative;    /* 1‑based rank within block */
    int rem0 = k, rem1 = k, rem2 = k;
    seq_t result = 0;

    for (int pos = 0; pos < 3 * k; ++pos) {
        for (int d = 0; d <= 2; ++d) {
            if (d == 0 && rem0 == 0) continue;
            if (d == 1 && rem1 == 0) continue;
            if (d == 2 && rem2 == 0) continue;

            int nrem0 = rem0 - (d == 0);
            int nrem1 = rem1 - (d == 1);
            int nrem2 = rem2 - (d == 2);

            int used0 = k - nrem0;
            int used1 = k - nrem1;
            int used2 = k - nrem2;

            if (!(used2 >= used1 && used1 >= used0)) continue;

            idx_t avail = count_dp(nrem0, nrem1, nrem2, k, dp);
            if (rank > avail) {
                rank -= avail;
            } else {
                result = result * 10 + d;
                rem0 = nrem0;
                rem1 = nrem1;
                rem2 = nrem2;
                break;
            }
        }
    }

    free(dp);
    return result;
}
