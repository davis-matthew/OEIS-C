/*
 * A207962  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:
   Transfer‑matrix DP counting 4×n binary arrays avoiding
   horizontally 000 and 101,
   and vertically 001 and 100.
   The states are 4‑bit column configurations that satisfy
   the vertical restrictions; 9 such configurations exist.
   Transitions from (prevprev, prev) to (prev, next) are allowed
   iff for every row the 3‑column window (prevprev, prev, next)
   does not contain 000 or 101.  The DP runs in O(n·9³) time. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* ---------- static data, built once ---------- */
    static bool built = false;
    static int   M;                /* number of vertical‑valid patterns (9)     */
    static int   patterns[9];      /* the patterns themselves                  */
    static bool  trans[9][9][9];   /* transition matrix: trans[i][j][k] = 1 if
                                     (i,j,k) allowed per horizontal rule       */

    if (!built) {
        /* Find all 4‑bit patterns that satisfy the vertical constraints
           (no 001 or 100 in any 3 consecutive rows). */
        int cnt = 0;
        for (int p = 0; p < 16; ++p) {
            bool ok = true;
            for (int i = 0; i < 2; ++i) {          /* two windows of 3 rows */
                int b0 = (p >> i)     & 1;
                int b1 = (p >> (i+1)) & 1;
                int b2 = (p >> (i+2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 1 && b1 == 0 && b2 == 0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                patterns[cnt] = p;
                ++cnt;
            }
        }
        M = cnt;               /* should be 9 */

        /* Precompute horizontal transition matrix */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                for (int k = 0; k < M; ++k) {
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        int a = (patterns[i] >> r) & 1;
                        int b = (patterns[j] >> r) & 1;
                        int c = (patterns[k] >> r) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 1 && b == 0 && c == 1)) {
                            ok = false;
                            break;
                        }
                    }
                    trans[i][j][k] = ok;
                }
        built = true;
    }

    /* --------- base cases --------- */
    if (n == 1) return (seq_t)(M);
    if (n == 2) return (seq_t)(M) * (seq_t)(M);

    /* --------- DP for n >= 3 --------- */
    seq_t dp[9][9];     /* dp[prevprev][prev] */
    seq_t newdp[9][9];

    /* Initialize for n = 2 (all pairs allowed) */
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            dp[i][j] = (seq_t)1;

    for (idx_t col = 3; col <= n; ++col) {
        /* Zero newdp */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                newdp[i][j] = 0;

        /* Transition */
        for (int i = 0; i < M; ++i) {            /* prevprev */
            for (int j = 0; j < M; ++j) {        /* prev    */
                seq_t val = dp[i][j];
                if (val == 0) continue;
                for (int k = 0; k < M; ++k) {    /* next    */
                    if (trans[i][j][k]) {
                        newdp[j][k] += val;
                    }
                }
            }
        }

        /* Copy newdp back to dp for next iteration */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum all states to get the final count */
    seq_t total = 0;
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            total += dp[i][j];

    return total;
}
