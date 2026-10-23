/*
 * A184352  Number of strings of numbers x(i=1..6) in 0..n with sum i^4*x(i)
 *          equal to 1296*n.
 * https://oeis.org/A184352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of ∑_{i=1..6} i^4·x(i) = 1296·n with
 *            0 ≤ x(i) ≤ n. The algorithm is a bounded‑knapsack DP on the
 *            variables x(2)..x(5) (coefficients 16,81,256,625). For each x(6)
 *            it checks which sums S of the remaining variables leave a
 *            remainder r = 1296·n – S – 1296·x(6) in the interv...
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* DP range: maximum possible sum of x(2..5) */
    idx_t L = 978 * n;                     /* 16+81+256+625 = 978 */
    seq_t *dp   = (seq_t*)calloc((size_t)(L + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;

    dp[0] = (seq_t)1;                      /* base case: zero sum */

    /* Coefficients for x(2)..x(5) */
    const idx_t coeffs[4] = {16, 81, 256, 625};

    for (int var = 0; var < 4; ++var) {
        idx_t c = coeffs[var];
        seq_t *newdp = (seq_t*)calloc((size_t)(L + 1), sizeof(seq_t));
        if (!newdp) {
            free(dp);
            return (seq_t)0;
        }

        /* Sliding‑window bounded knapsack for this coefficient */
        for (idx_t r = 0; r < c; ++r) {
            seq_t sum = 0;
            idx_t  k   = 0;
            for (idx_t i = r; i <= L; i += c, ++k) {
                sum += dp[i];
                if (k > n)         /* discard the (k-n‑1)-th term */
                    sum -= dp[i - (n + 1) * c];
                newdp[i] = sum;
            }
        }

        free(dp);
        dp = newdp;
    }

    /* Convert dp into prefix sums (dp now acts as pref) */
    for (idx_t i = 1; i <= L; ++i)
        dp[i] += dp[i - 1];

    seq_t total = 0;

    /* Enumerate all possibilities for x(6) */
    for (idx_t x6 = 0; x6 <= n; ++x6) {
        idx_t M      = n - x6;                  /* remaining sum multiplier */
        idx_t maxS   = 1296 * M;                /* 1296*(n - x6) */
        idx_t low    = (maxS >= n) ? (maxS - n) : 0;
        if (low > L)
            continue;                          /* no achievable S in range */

        idx_t high = maxS;
        if (high > L)
            high = L;

        seq_t ways = dp[high];
        if (low > 0)
            ways -= dp[low - 1];
        total += ways;
    }

    free(dp);
    return total;
}
