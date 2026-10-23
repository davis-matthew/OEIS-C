/*
 * A189691  Number of nX4 binary arrays without the pattern 0 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A189691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP for A189691: number of n×4 binary matrices avoiding the pattern 0 0 1
 * vertically, diagonally, or antidiagonally. */
#include <stdbool.h>
seq_t a(idx_t n)
{
    /* handle offset */
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)256;

    /* static transition table: trans[state][next] == 1 if
     * the triplet (prev2,prev1,next) is allowed. */
    static unsigned char trans[256][16];
    static bool init = false;
    if (!init) {
        for (int p2 = 0; p2 < 16; ++p2) {
            for (int p1 = 0; p1 < 16; ++p1) {
                int state = (p2 << 4) | p1;
                for (int r = 0; r < 16; ++r) {
                    bool ok = true;

                    /* vertical: no 0 0 1 in any column */
                    for (int j = 0; j < 4; ++j) {
                        if (((p2 >> j) & 1) == 0 &&
                            ((p1 >> j) & 1) == 0 &&
                            ((r >> j) & 1) == 1) {
                            ok = false; break;
                        }
                    }
                    if (!ok) { trans[state][r] = 0; continue; }

                    /* diagonal down‑right */
                    for (int j = 0; j <= 1; ++j) {
                        if (((p2 >> j) & 1) == 0 &&
                            ((p1 >> (j + 1)) & 1) == 0 &&
                            ((r >> (j + 2)) & 1) == 1) {
                            ok = false; break;
                        }
                    }
                    if (!ok) { trans[state][r] = 0; continue; }

                    /* anti‑diagonal down‑left */
                    for (int j = 0; j <= 1; ++j) {
                        if (((p2 >> (j + 2)) & 1) == 0 &&
                            ((p1 >> (j + 1)) & 1) == 0 &&
                            ((r >> j) & 1) == 1) {
                            ok = false; break;
                        }
                    }
                    trans[state][r] = ok ? 1 : 0;
                }
            }
        }
        init = true;
    }

    seq_t dp[256];
    seq_t ndp[256];

    /* initial state after two rows: each pair (prev2,prev1) occurs once */
    for (int s = 0; s < 256; ++s) dp[s] = 1;

    for (idx_t i = 3; i <= n; ++i) {
        for (int s = 0; s < 256; ++s) ndp[s] = 0;
        for (int s = 0; s < 256; ++s) {
            seq_t cnt = dp[s];
            if (cnt == 0) continue;
            int p2 = s >> 4;
            int p1 = s & 0xF;
            int base = (p1 << 4);
            for (int r = 0; r < 16; ++r) {
                if (trans[s][r]) ndp[base | r] += cnt;
            }
        }
        for (int s = 0; s < 256; ++s) dp[s] = ndp[s];
    }

    seq_t total = 0;
    for (int s = 0; s < 256; ++s) total += dp[s];
    return total;
}
