/*
 * A234781  Number of (n+1)X(3+1) 0..3 arrays with no adjacent elements equal
 *          and with each 2X2 subblock having the number of clockwise edge
 *          increases equal to the number of counterclockwise edge increases.
 * https://oeis.org/A234781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by dynamic programming on the 108 admissible
 *            4‑tuples (rows). A(n) counts (n+1)×4 arrays over {0,1,2,3} with
 *            no equal adjacent entries and with each 2×2 subblock having the
 *            same number of clockwise and counter‑clockwise edge increases
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required headers.  */
#include <stdint.h>
#include <stdbool.h>

/*  Number of admissible row patterns (no equal neighbours).  */
enum { NUM_PATTERNS = 108 };

/*  Public function required by the framework.  */
seq_t a(idx_t n)
{
    /*  Store the pattern list and the adjacency matrix
        on the first call.  */
    static uint8_t patt[NUM_PATTERNS][4];
    static bool    adj[NUM_PATTERNS][NUM_PATTERNS];
    static bool    built = false;

    if (!built) {
        /*  Build all 4‑tuples with no equal neighbours.  */
        int idx = 0;
        for (int a0 = 0; a0 < 4; ++a0) {
            for (int a1 = 0; a1 < 4; ++a1) if (a1 != a0) {
                for (int a2 = 0; a2 < 4; ++a2) if (a2 != a1) {
                    for (int a3 = 0; a3 < 4; ++a3) if (a3 != a2) {
                        patt[idx][0] = (uint8_t)a0;
                        patt[idx][1] = (uint8_t)a1;
                        patt[idx][2] = (uint8_t)a2;
                        patt[idx][3] = (uint8_t)a3;
                        ++idx;
                    }
                }
            }
        }

        /*  Pre‑compute adjacency: pattern i → pattern j is allowed if
            (i) all vertical neighbours differ, and
            (ii) each of the 3 2×2 blocks between the two rows has
                exactly two clockwise (equivalently, two counter‑clockwise)
                edge increases.  */
        for (int i = 0; i < NUM_PATTERNS; ++i) {
            const uint8_t *p = patt[i];
            for (int j = 0; j < NUM_PATTERNS; ++j) {
                const uint8_t *q = patt[j];
                bool ok = true;

                /*  Check vertical adjacency  */
                for (int c = 0; c < 4; ++c) {
                    if (p[c] == q[c]) { ok = false; break; }
                }
                if (!ok) { adj[i][j] = false; continue; }

                /*  Check 2×2 blocks  */
                for (int c = 0; c < 3 && ok; ++c) {
                    uint8_t a0 = p[c],   a1 = p[c+1];
                    uint8_t b0 = q[c],   b1 = q[c+1];
                    int cnt = 0;
                    if (a0 < a1) ++cnt;   /* top edge */
                    if (a1 < b1) ++cnt;   /* right edge */
                    if (b1 < b0) ++cnt;   /* bottom edge */
                    if (b0 < a0) ++cnt;   /* left edge */
                    if (cnt != 2) ok = false;
                }
                adj[i][j] = ok;
            }
        }
        built = true;
    }

    /*  Dynamic programming over rows.  */
    seq_t dp_cur[NUM_PATTERNS];
    seq_t dp_next[NUM_PATTERNS];

    /*  Initial row: all patterns are possible.  */
    for (int v = 0; v < NUM_PATTERNS; ++v) dp_cur[v] = 1;

    /*  Perform n transitions (to obtain n+1 rows).  */
    for (idx_t step = 0; step < n; ++step) {
        for (int v = 0; v < NUM_PATTERNS; ++v) dp_next[v] = 0;
        for (int i = 0; i < NUM_PATTERNS; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            for (int j = 0; j < NUM_PATTERNS; ++j) {
                if (adj[i][j]) dp_next[j] += val;
            }
        }
        /*  Copy next state to current.  */
        for (int v = 0; v < NUM_PATTERNS; ++v) dp_cur[v] = dp_next[v];
    }

    /*  Sum counts over all possible last rows.  */
    seq_t total = 0;
    for (int v = 0; v < NUM_PATTERNS; ++v) total += dp_cur[v];
    return total;
}
