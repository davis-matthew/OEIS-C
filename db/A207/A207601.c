/*
 * A207601  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays avoiding the patterns
 *            horizontally 0 0 0 and 0 1 0, and vertically 0 0 1 and 1 0 1.
 *            Initial values: a(1)=8, a(2)=64, a(3)=168, a(4)=441, …
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n==0; return 0 */

    /* Special cases with trivial answers */
    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)64;

    /* Static tables describing the allowed columns (8 of them) and
       all legal transitions between pairs of columns. */
    static int  init_done = 0;
    static uint8_t allowed_next[64][8];
    static uint8_t allowed_next_count[64];
    static uint8_t rows[8][4];

    if (!init_done) {
        /* List of the 8 allowed column patterns (4‑bit values). */
        const int allowed_cols[8] = {0, 4, 6, 7, 8, 12, 14, 15};

        /* Fill the bit table for each allowed column. */
        for (int i = 0; i < 8; ++i) {
            int col = allowed_cols[i];
            for (int r = 0; r < 4; ++r)
                rows[i][r] = (col >> r) & 1;
        }

        /* Pre‑compute allowed_next transitions for each pair of columns. */
        for (int i = 0; i < 64; ++i) {        /* pair index = c1*8 + c2 */
            int c1 = i / 8;
            int c2 = i % 8;
            allowed_next_count[i] = 0;
            for (int c3 = 0; c3 < 8; ++c3) {
                int ok = 1;
                for (int r = 0; r < 4; ++r) {
                    int b1 = rows[c1][r];
                    int b2 = rows[c2][r];
                    int b3 = rows[c3][r];
                    int triple = (b1 << 2) | (b2 << 1) | b3;
                    if (triple == 0 || triple == 2) { /* forbidden 000 or 010 */
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    allowed_next[i][allowed_next_count[i]++] = c3;
                }
            }
        }
        init_done = 1;
    }

    /* DP arrays: counts of sequences ending with a specific pair of columns. */
    seq_t prev[64];
    seq_t cur[64];

    /* Start with length 2: every allowed pair occurs exactly once. */
    for (int i = 0; i < 64; ++i)
        prev[i] = (seq_t)1;   /* count for pair (c1,c2) */

    /* Iterate from length 3 up to the requested n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Reset current counts. */
        for (int i = 0; i < 64; ++i)
            cur[i] = (seq_t)0;

        /* Transition from prev to cur. */
        for (int i = 0; i < 64; ++i) {
            seq_t val = prev[i];
            if (val == (seq_t)0)
                continue;
            int c1 = i / 8;
            int c2 = i % 8;
            int cnt = allowed_next_count[i];
            for (int t = 0; t < cnt; ++t) {
                int c3 = allowed_next[i][t];
                int nextIdx = c2 * 8 + c3;   /* new pair (c2,c3) */
                cur[nextIdx] += val;
            }
        }

        /* Sum all current pair counts to get a(len). */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 64; ++i)
            total += cur[i];

        if (len == n)            /* reached requested length */
            return total;

        /* Prepare for next iteration: cur becomes prev. */
        for (int i = 0; i < 64; ++i)
            prev[i] = cur[i];
    }

    return (seq_t)0;   /* should never be reached */
}
