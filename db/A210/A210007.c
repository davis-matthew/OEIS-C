/*
 * A210007  Number of (n+1)X4 0..2 arrays containing all values 0..2 with
 *          every 2X2 subblock having two or three distinct values, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A210007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   inc_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming that counts all (n+1)×4 arrays of entries 0,1,2
   containing each value at least once, such that no 2×2 subblock contains all four
   equal values and new values appear in row-major order.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/inc_ok.h"

/* Transition structure for DP */
typedef struct { uint8_t pat; uint8_t newmask; } Transition;

/* Main function computing A210007 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* ---------- static data, initialized once ---------- */
    static bool init_done = false;
    static uint8_t pat[81][4];
    static bool ok2x2[81][81];
    static Transition tr[648][81];
    static int trcnt[648];

    if (!init_done) {
        /* enumerate all 4‑digit base‑3 patterns */
        for (int p = 0; p < 81; ++p) {
            int x = p;
            for (int i = 3; i >= 0; --i) {
                pat[p][i] = x % 3;
                x /= 3;
            }
        }

        /* precompute which pattern pairs satisfy the 2×2 subblock constraint */
        for (int a = 0; a < 81; ++a) {
            for (int b = 0; b < 81; ++b) {
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    uint8_t p0 = pat[a][j];
                    uint8_t p1 = pat[a][j + 1];
                    uint8_t c0 = pat[b][j];
                    uint8_t c1 = pat[b][j + 1];
                    if (p0 == p1 && p0 == c0 && p0 == c1) {
                        ok = false;
                        break;
                    }
                }
                ok2x2[a][b] = ok;
            }
        }

        /* precompute transition lists for each state (prev_pattern, prev_mask) */
        for (int a = 0; a < 81; ++a) {
            for (int m = 0; m < 8; ++m) {
                int s = a * 8 + m;
                trcnt[s] = 0;
                for (int b = 0; b < 81; ++b) {
                    if (!ok2x2[a][b]) continue;
                    uint8_t newm;
                    if (inc_ok((uint8_t)m, pat[b], &newm)) {
                        tr[s][trcnt[s]].pat = (uint8_t)b;
                        tr[s][trcnt[s]].newmask = newm;
                        trcnt[s] += 1;
                    }
                }
            }
        }
        init_done = true;
    }

    /* ---------- DP arrays ---------- */
    static seq_t dp_cur[81][8];
    static seq_t dp_next[81][8];

    /* zero dp_cur */
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 8; ++j)
            dp_cur[i][j] = 0;

    /* ---- initial row (row 0) ---- */
    uint8_t init_mask = 1;   /* only value 0 seen */
    for (int b = 0; b < 81; ++b) {
        if (pat[b][0] != 0) continue;          /* first cell must be 0 */
        uint8_t newm;
        if (inc_ok(init_mask, pat[b], &newm))
            dp_cur[b][newm] += 1;
    }

    /* ---- process remaining n rows (row 1 … n) ---- */
    for (idx_t r = 1; r <= n; ++r) {
        /* zero dp_next */
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 8; ++j)
                dp_next[i][j] = 0;

        for (int a = 0; a < 81; ++a) {
            for (int m = 0; m < 8; ++m) {
                seq_t val = dp_cur[a][m];
                if (val == 0) continue;
                int s = a * 8 + m;
                int cnt = trcnt[s];
                for (int t = 0; t < cnt; ++t) {
                    uint8_t b = tr[s][t].pat;
                    uint8_t newm = tr[s][t].newmask;
                    dp_next[b][newm] += val;
                }
            }
        }

        /* copy dp_next to dp_cur for next iteration */
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 8; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* ---- sum over states with all three values seen (mask == 7) ---- */
    seq_t ans = 0;
    for (int a = 0;  a < 81; ++a)
        ans += dp_cur[a][7];

    return ans;
}
