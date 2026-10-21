/*
 * A207861  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207861:
   Number of 6 × n binary arrays avoiding
   horizontally the patterns 000 and 010,
   vertically the patterns 010 and 101.
   This function computes the value by dynamic programming
   over the 26 admissible 6‑bit column vectors. */

#include <stdbool.h>

/* No typedefs for seq_t and idx_t; they are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* Pre‑compute the admissible column vectors (6‑bit numbers). */
    static int col[26];
    static int colcnt = 0;
    static bool col_ok[64];
    static bool initialized = false;

    if (!initialized) {
        colcnt = 0;
        for (int c = 0; c < 64; ++c) {
            bool ok = true;
            /* check all vertical triples in this column */
            for (int r = 0; r < 4; ++r) {
                int t = ((c >> r)     & 1) << 2 |
                        ((c >> (r+1)) & 1) << 1 |
                        ((c >> (r+2)) & 1);
                if (t == 0b010 || t == 0b101) { ok = false; break; }
            }
            col_ok[c] = ok;
            if (ok) col[colcnt++] = c;
        }
        initialized = true;
    }

    /* The number of admissible columns is 26. */
    seq_t base = (seq_t)colcnt;
    if (n == 1) return base;
    if (n == 2) return base * base;

    /* Transition tables: for each pair of previous columns (i,j)
       list all admissible next columns k that keep every row
       from forming 000 or 010. */
    static int nextcnt[26*26];
    static int next[26*26][26];
    static bool trans_ok = false;

    if (!trans_ok) {
        for (int i = 0; i < colcnt; ++i) {
            for (int j = 0; j < colcnt; ++j) {
                int s = i * colcnt + j;
                int c = 0;
                int a = col[i];
                int b = col[j];
                for (int k = 0; k < colcnt; ++k) {
                    int d = col[k];
                    bool ok = true;
                    for (int r = 0; r < 6; ++r) {
                        int t = ((a >> r) & 1) << 2 |
                                ((b >> r) & 1) << 1 |
                                ((d >> r) & 1);
                        if (t == 0b000 || t == 0b010) { ok = false; break; }
                    }
                    if (ok) next[s][c++] = k;
                }
                nextcnt[s] = c;
            }
        }
        trans_ok = true;
    }

    /* Dynamic programming over columns 3 … n. */
    static seq_t cur[26*26];
    static seq_t nxt[26*26];

    /* initialise for n = 2: each pair of admissible columns is a valid start. */
    for (int s = 0; s < colcnt * colcnt; ++s)
        cur[s] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < colcnt * colcnt; ++s)
            nxt[s] = (seq_t)0;

        for (int s = 0; s < colcnt * colcnt; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            int j = s % colcnt;          /* second column of the pair */
            int cnt = nextcnt[s];
            for (int t = 0; t < cnt; ++t) {
                int k = next[s][t];
                int ns = j * colcnt + k;
                nxt[ns] += val;
            }
        }

        /* copy nxt → cur for next iteration */
        for (int s = 0; s < colcnt * colcnt; ++s)
            cur[s] = nxt[s];
    }

    /* Sum over all states to obtain the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int s = 0; s < colcnt * colcnt; ++s)
        total += cur[s];

    return total;
}
