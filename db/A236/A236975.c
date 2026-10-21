/*
 * A236975  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the upper
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×(3+1) 0..3 arrays whose * for every 2×2
 *            subblock has the same value of * (maximum + upper median –
 *            minimum)
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   expr4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/expr4.h"

seq_t a(idx_t n)
{
    /* Static tables (initialised once). */
    static bool init = false;
    static unsigned char rows[256][4];
    static int out_deg[7][256];
    static int out_edge[7][256][256];

    if (!init) {
        /* Encode each of the 256 rows. */
        for (int r = 0; r < 256; ++r) {
            for (int c = 0; c < 4; ++c)
                rows[r][c] = (unsigned char)((r >> (2 * c)) & 3);
        }

        /* Build transition graph for each possible T (0..6). */
        for (int t = 0; t < 7; ++t)
            for (int i = 0; i < 256; ++i)
                out_deg[t][i] = 0;

        for (int r1 = 0; r1 < 256; ++r1) {
            for (int r2 = 0; r2 < 256; ++r2) {
                int e[3];
                for (int j = 0; j < 3; ++j) {
                    int a = rows[r1][j];
                    int b = rows[r1][j + 1];
                    int c = rows[r2][j];
                    int d = rows[r2][j + 1];
                    e[j] = expr4(a, b, c, d);
                }
                if (e[0] == e[1] && e[1] == e[2]) {
                    int t = e[0];
                    out_edge[t][r1][out_deg[t][r1]++] = r2;
                }
            }
        }
        init = true;
    }

    seq_t total = 0;
    idx_t steps = n;                      /* number of transitions = n */
    for (int t = 0; t < 7; ++t) {
        seq_t cur[256], nxt[256];
        for (int i = 0; i < 256; ++i)
            cur[i] = (seq_t)1;            /* any first row allowed */

        for (idx_t s = 0; s < steps; ++s) {
            for (int i = 0; i < 256; ++i)
                nxt[i] = (seq_t)0;
            for (int r = 0; r < 256; ++r) {
                seq_t val = cur[r];
                if (!val) continue;
                int deg = out_deg[t][r];
                for (int k = 0; k < deg; ++k) {
                    int nr = out_edge[t][r][k];
                    nxt[nr] += val;
                }
            }
            for (int i = 0; i < 256; ++i)
                cur[i] = nxt[i];
        }

        for (int i = 0; i < 256; ++i)
            total += cur[i];
    }
    return total;
}
