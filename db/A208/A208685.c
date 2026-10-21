/*
 * A208685  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding horizontally the
 *            triples 000 and 001 and vertically the triples 011 and 101. The
 *            algorithm builds all 5‑bit rows that satisfy the horizontal
 *            rule. Then for each pair of consecutive rows we precompute which
 *            next rows are allowed by the vertical rule. Fin...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static data for the automaton – computed once */
    static int rows[32];          /* all 5‑bit rows that satisfy horizontal rule */
    static int m;                 /* number of such rows (m ≤ 16) */
    static uint16_t trans[16][16];/* mask of allowed next rows for each (prevprev,prev) pair */
    static bool initialized = false;

    /* initialise only once */
    if (!initialized) {
        int idx = 0;
        /* mapping from 5‑bit pattern to its index in rows (or -1) */
        int pat2idx[32];
        for (int i = 0; i < 32; ++i) pat2idx[i] = -1;

        /* collect allowed rows (no two consecutive zeros) */
        for (int pat = 0; pat < 32; ++pat) {
            bool ok = true;
            for (int i = 0; i < 3; ++i) {
                int b0 = (pat >> i)      & 1;
                int b1 = (pat >> (i+1))  & 1;
                if (b0 == 0 && b1 == 0) { ok = false; break; }
            }
            if (ok) {
                rows[idx] = pat;
                pat2idx[pat] = idx;
                ++idx;
            }
        }
        m = idx;   /* m should be 16 */

        /* precompute valid triples */
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                int a = rows[i];
                int b = rows[j];
                uint16_t mask = 0;
                for (int k = 0; k < m; ++k) {
                    int c = rows[k];
                    bool ok = true;
                    for (int col = 0; col < 5 && ok; ++col) {
                        int x = (a >> col) & 1;
                        int y = (b >> col) & 1;
                        int z = (c >> col) & 1;
                        if ((x == 0 && y == 1 && z == 1) ||
                            (x == 1 && y == 0 && z == 1))
                            ok = false;
                    }
                    if (ok) mask |= (1u << k);
                }
                trans[i][j] = mask;
            }
        }

        initialized = true;
    }

    /* base cases */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)m;
    if (n == 2) return (seq_t)m * (seq_t)m;

    /* dynamic programming: dp[i*m + j] counts ways ending with rows i (prevprev) and j (prev) */
    seq_t dp[256] = {0};
    seq_t ndp[256] = {0};

    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            dp[i*m + j] = 1;

    for (idx_t r = 3; r <= n; ++r) {
        /* reset ndp */
        for (size_t idx = 0; idx < (size_t)m*m; ++idx)
            ndp[idx] = 0;

        /* transition */
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                seq_t ways = dp[i*m + j];
                uint16_t mask = trans[i][j];
                for (int k = 0; k < m; ++k) {
                    if (mask & (1u << k))
                        ndp[j*m + k] += ways;
                }
            }
        }
        /* copy ndp to dp */
        for (size_t idx = 0; idx < (size_t)m*m; ++idx)
            dp[idx] = ndp[idx];
    }

    /* sum all ending states */
    seq_t total = 0;
    for (size_t idx = 0; idx < (size_t)m*m; ++idx)
        total += dp[idx];

    return total;
}
