/*
 * A229707  Triangular array read by rows. T(n,k) is the number of strictly
 *          unimodal compositions of n with the greatest part equal to k;
 *          n>=1, 1<=k<=n.
 * https://oeis.org/A229707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened triangular array *
 *            T(r,k) of strictly unimodal compositions: * Let r be such that
 *            r(r‑1)/2 < n ≤ r(r+1)/2, and k = n – r(r‑1)/2. * Then a(n) =
 *            T(r,k), where * T(r,k) = #{ (L,R) | L,R ⊆ {1,…,k‑1},
 *            sum(L)+sum(R) = r‑k }. * That is, the number of pairs of su...
 * Range:     u8 n <= 283, u16 n <= 1553, u32 n <= 9312, u64 n <= 9312,
 *            u128 n <= 9312, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, but idx_t is defined by the build */

seq_t a(idx_t n)
{
    /* Find the triangular row r and column k that correspond to index n */
    idx_t r = 0;
    idx_t cumulative = 0;          /* cumulative sum of 1..r */
    while (cumulative < n) {
        ++r;
        cumulative += r;           /* add current r */
    }
    /* Now cumulative = r(r+1)/2  and  n > (r-1)r/2 = cumulative - r */
    idx_t prev_sum = cumulative - r;   /* sum of 1..(r-1) */
    idx_t k = n - prev_sum;            /* 1 <= k <= r */

    /* Each value of k uses a DP array up to max_s = r-k */
    idx_t max_s = r - k;               /* maximum sum needed from subsets */

    /* Static array large enough for all calls (r <= 141 for n <= 10000) */
    static seq_t dp[200];
    /* Initialize dp[0..max_s] to 0 */
    for (idx_t i = 0; i <= max_s; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;                         /* empty subset */

    /* Build the subset-sum counts for {1,…,k-1} */
    for (idx_t item = 1; item <= k - 1; ++item) {
        if (item > max_s) break;       /* no contribution beyond max_s */
        for (idx_t s = max_s; s >= item; --s) {
            dp[s] += dp[s - item];
        }
    }

    /* Compute convolution: sum_{s=0}^{max_s} dp[s] * dp[max_s-s] */
    seq_t result = 0;
    for (idx_t s = 0; s <= max_s; ++s) {
        result += dp[s] * dp[max_s - s];
    }

    return result;
}
