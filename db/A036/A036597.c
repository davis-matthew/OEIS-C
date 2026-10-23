/*
 * A036597  Number of binary rooted trees with n nodes and height exactly 8.
 * https://oeis.org/A036597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = T(n,8) - T(n,7) where * T(n,h) counts unordered binary
 *            rooted trees with n nodes and height ≤ h. * (The recurrence uses
 *            the standard split of the two subtrees.)
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 37, u64 n <= 89, u128 n <= 255,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_N 20000        /* reasonable upper bound for expected inputs */

static seq_t dp[9][MAX_N + 1];  /* dp[h][n]  (h = 0..8, n = 0..MAX_N) */
static idx_t max_n = 0;         /* maximum n that has been computed */

/* extend precomputation up to n (if necessary) */
static void _ensure_upto(idx_t n)
{
    if (n <= max_n) return;
    if (n > MAX_N) {
        /* Out‑of‑range request – silently ignore beyond MAX_N. */
        if (n > MAX_N) n = MAX_N;
    }

    for (idx_t nn = max_n + 1; nn <= n; ++nn) {
        /* h = 0 never used but keep zeroed                            */
        /* h = 1 : only a single node tree of size 1                    */
        if (nn == 1) dp[1][nn] = 1;
        else dp[1][nn] = 0;

        for (int h = 2; h <= 8; ++h) {
            seq_t val = 0;
            if (nn >= 2) val += dp[h-1][nn-1];          /* one child */

            if (nn >= 3) {
                idx_t limit = (nn - 1) / 2;             /* a ≤ b          */
                for (idx_t a = 1; a <= limit; ++a) {
                    idx_t b = nn - 1 - a;
                    seq_t left  = dp[h-1][a];
                    seq_t right = dp[h-1][b];
                    if (a == b)
                        val += left * (left + 1) / 2;   /* unordered pair */
                    else
                        val += left * right;
                }
            }
            dp[h][nn] = val;
        }
    }

    /* Ensure base case for h >= 2: a single node tree has height 1 */
    for (int h = 2; h <= 8; ++h) dp[h][1] = 1;

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    _ensure_upto(n);
    return dp[8][n] - dp[7][n];
}
