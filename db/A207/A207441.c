/*
 * A207441  Number of nX7 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding the horizontal *
 *            patterns 0 0 1 and 0 1 0 and the vertical patterns 0 0 1 and 1 0
 *            0. * The function uses a DP over the last two rows; a(1) = 31,
 *            a(2) = 961, * and the recurrence is * a(n) = Σ_{i,j,k}
 *            a(n‑2)_{i,j} for all transitions (i,j,k) allowed * wh...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static initialization flag */
    static int initialized = 0;

    /* constants for the problem */
    static int M;                    /* number of valid 7‑bit rows (≤31) */
    static uint8_t rows[128];        /* all 7‑bit patterns that satisfy
                                        the horizontal constraints */
    /* transition masks: nextmask[i][j] has bit k set iff
       rows[i], rows[j], rows[k] satisfy the vertical constraints */
    static uint32_t nextmask[32][32];
    /* DP tables (size M*M ≤ 961) */
    static seq_t dp[961];
    static seq_t nextdp[961];

    /* ----- initialise the static data once ----- */
    if (!initialized) {
        /* find all valid 7‑bit rows (no 0 0 1 or 0 1 0 horizontally) */
        int m = 0;
        for (int p = 0; p < 128; ++p) {
            int ok = 1;
            for (int j = 0; j <= 4 && ok; ++j) {
                int b0 = (p >> j) & 1;
                int b1 = (p >> (j + 1)) & 1;
                int b2 = (p >> (j + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 0 && b1 == 1 && b2 == 0))
                    ok = 0;
            }
            if (ok)
                rows[m++] = (uint8_t)p;
        }
        M = m;    /* should be 31 */

        /* pre‑compute transition masks */
        for (int i = 0; i < M; ++i) {
            uint8_t prev = rows[i];
            for (int j = 0; j < M; ++j) {
                uint8_t cur = rows[j];
                uint32_t mask = 0;
                for (int k = 0; k < M; ++k) {
                    uint8_t nxt = rows[k];
                    int good = 1;
                    for (int c = 0; c < 7 && good; ++c) {
                        int a = (prev >> c) & 1;
                        int b = (cur >> c) & 1;
                        int ccc = (nxt >> c) & 1;
                        if ((a == 0 && b == 0 && ccc == 1) ||
                            (a == 1 && b == 0 && ccc == 0))
                            good = 0;
                    }
                    if (good)
                        mask |= (1u << k);
                }
                nextmask[i][j] = mask;
            }
        }
        initialized = 1;
    }

    /* ----- base cases ----- */
    if (n == 0) return (seq_t)0;          /* not defined for offset 1 */
    if (n == 1) return (seq_t)M;
    if (n == 2) return (seq_t)M * (seq_t)M;

    /* ----- dynamic programming for n ≥ 3 ----- */
    int S = M * M;                            /* number of (prev,cur) states */

    /* initialise dp for two rows: every pair of valid rows is allowed */
    for (int s = 0; s < S; ++s) dp[s] = 0;
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            dp[i * M + j] = (seq_t)1;

    /* iterate rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear nextdp */
        for (int s = 0; s < S; ++s) nextdp[s] = 0;

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t ways = dp[i * M + j];
                if (ways == 0) continue;
                uint32_t mask = nextmask[i][j];
                /* iterate over allowed k */
                for (int k = 0; k < M; ++k) {
                    if (mask & (1u << k))
                        nextdp[j * M + k] += ways;
                }
            }
        }
        /* swap dp and nextdp */
        for (int s = 0; s < S; ++s) dp[s] = nextdp[s];
    }

    /* sum over all final states */
    seq_t total = 0;
    for (int s = 0; s < S; ++s)
        total += dp[s];

    return total;
}
