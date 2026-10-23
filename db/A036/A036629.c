/*
 * A036629  Number of 4-ary rooted trees with n nodes and height exactly 5.
 * https://oeis.org/A036629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of 4‑ary rooted trees with n nodes * and height
 *            exactly 5 (height is the number of nodes on a longest *
 *            root‑to‑leaf path). The value is computed as * a(n) = F[5][n] –
 *            F[4][n] * where F[h][n] is the number of 4‑ary rooted trees with
 *            n nodes * and height ≤ h. The computation uses a d...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 37, u64 n <= 90, u128 n <= 340,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_NODES 341              /* 1 + 4 + 16 + 64 + 256 = 341 */

/* local storage for dynamic programming */
static seq_t cache[MAX_NODES + 1];
static bool built = false;

/* Small binomial coefficients for repetitions of types.
   bin[c] = C(T + c - 1, c)  (number of multisets of size c from T types). */
static void binom_small(seq_t T, seq_t bin[5])
{
    bin[0] = 1;
    for (int c = 1; c <= 4; ++c)
        bin[c] = bin[c - 1] * (T + c - 1) / c;
}

/* Build all tables F[h][n] for h = 1..5.  For h=1, only the single‑node
   tree exists.  The recurrence for h>1 uses multi‑set addition of
   subtrees from height ≤ h-1. */
static void compute(void)
{
    /* F[1][n] */
    static seq_t f_prev[MAX_NODES + 1];
    static seq_t f_cur[MAX_NODES + 1];
    static seq_t f4[MAX_NODES + 1];
    static seq_t f5[MAX_NODES + 1];

    for (idx_t i = 0; i <= MAX_NODES; ++i) f_prev[i] = 0;
    f_prev[1] = 1;                /* height ≤ 1: only the root */
    for (idx_t i = 0; i <= MAX_NODES; ++i) cache[i] = 0;

    for (int h = 2; h <= 5; ++h) {
        /* DP: dp[k][s] counts ways to choose k subtrees
           whose total node count is s, using only subtree sizes < current. */
        static seq_t dp[5][MAX_NODES];
        static seq_t dp2[5][MAX_NODES];

        for (int k = 0; k <= 4; ++k)
            for (idx_t s = 0; s < MAX_NODES; ++s)
                dp[k][s] = 0;
        dp[0][0] = 1;          /* start with no children */

        for (idx_t sz = 1; sz <= MAX_NODES; ++sz) {
            seq_t T = f_prev[sz];
            if (T == 0) continue;

            seq_t bin[5];
            binom_small(T, bin);

            for (int k = 0; k <= 4; ++k)
                for (idx_t s = 0; s < MAX_NODES; ++s)
                    dp2[k][s] = 0;

            for (int k = 0; k <= 4; ++k) {
                for (idx_t s = 0; s < MAX_NODES; ++s) {
                    seq_t cur_val = dp[k][s];
                    if (cur_val == 0) continue;
                    for (int c = 0; c <= 4 - k; ++c) {
                        idx_t new_s = s + c * sz;
                        if (new_s >= MAX_NODES) break;
                        seq_t add = cur_val * bin[c];
                        dp2[k + c][new_s] += add;
                    }
                }
            }

            for (int k = 0; k <= 4; ++k)
                for (idx_t s = 0; s < MAX_NODES; ++s)
                    dp[k][s] = dp2[k][s];
        }

        /* Construct f_cur[n] from dp: root plus any number of children. */
        for (idx_t n = 1; n <= MAX_NODES; ++n) {
            seq_t val = 0;
            idx_t rem = n - 1;      /* nodes in all children together */
            for (int k = 0; k <= 4; ++k)
                val += dp[k][rem];
            f_cur[n] = val;
        }

        /* Preserve results for heights 4 and 5. */
        if (h == 4)
            for (idx_t i = 0; i <= MAX_NODES; ++i) f4[i] = f_cur[i];
        if (h == 5)
            for (idx_t i = 0; i <= MAX_NODES; ++i) f5[i] = f_cur[i];

        /* Prepare for next height. */
        for (idx_t i = 0; i <= MAX_NODES; ++i) f_prev[i] = f_cur[i];
    }

    /* cache = F[5] - F[4] gives height exactly 5. */
    for (idx_t i = 0; i <= MAX_NODES; ++i)
        cache[i] = f5[i] - f4[i];
    built = true;
}

seq_t a(idx_t n)
{
    if (!built) compute();
    if (n <= MAX_NODES) return cache[n];
    return 0;
}
