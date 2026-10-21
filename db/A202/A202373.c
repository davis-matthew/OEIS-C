/*
 * A202373  Number of (n+2)X5 binary arrays avoiding patterns 001 and 111 in
 *          rows and columns.
 * https://oeis.org/A202373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 19, u64 n <= 55, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transfer‑matrix counting for A202373
 * Number of (n+2)×5 binary arrays avoiding 001 and 111 in rows and columns.
 * The function returns a(n) for n≥1. */

/* The algorithm uses a state graph on pairs of consecutive allowed rows
 * (row i , row i+1).  There are 13 rows that avoid 001 and 111
 * horizontally.  A transition from (r1,r2) to (r2,r3) is allowed if
 * in every column the triple (r1,r2,r3) does not contain 001 or 111.
 * The number of arrays of height n+2 is the number of length‑(n+2)
 * walks in this graph, which we compute by dynamic programming. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset starts at 1 */

    /* List of the 13 allowed 5‑bit rows (no 001 or 111 horizontally). */
    static const uint8_t allowed[13] = {
        0,   /* 00000 */
        8,   /* 01000 */
        10,  /* 01010 */
        11,  /* 01011 */
        12,  /* 01100 */
        13,  /* 01101 */
        16,  /* 10000 */
        20,  /* 10100 */
        21,  /* 10101 */
        22,  /* 10110 */
        24,  /* 11000 */
        26,  /* 11010 */
        27   /* 11011 */
    };

    /* Transition graph: 169 states = 13×13 pairs of rows.
     * adj[s][k] gives the k‑th outgoing neighbor of state s;
     * outdeg[s] is the number of outgoing edges from s.               */
    static int adj[169][13];
    static int outdeg[169];
    static bool built = false;

    if (!built) {
        for (int i = 0; i < 13; ++i) {
            int r1 = allowed[i];
            for (int j = 0; j < 13; ++j) {
                int r2 = allowed[j];
                int s = i * 13 + j;
                outdeg[s] = 0;
                for (int k = 0; k < 13; ++k) {
                    int r3 = allowed[k];
                    bool ok = true;
                    for (int bit = 0; bit < 5; ++bit) {
                        int b1 = (r1 >> bit) & 1;
                        int b2 = (r2 >> bit) & 1;
                        int b3 = (r3 >> bit) & 1;
                        if (b1 == 0 && b2 == 0 && b3 == 1) { ok = false; break; } /* 001 */
                        if (b1 == 1 && b2 == 1 && b3 == 1) { ok = false; break; } /* 111 */
                    }
                    if (ok) {
                        adj[s][outdeg[s]++] = j * 13 + k;
                    }
                }
            }
        }
        built = true;
    }

    /* DP arrays for current and next row counts. */
    static seq_t cur[169];
    static seq_t nxt[169];

    /* Initialize counts for the first two rows: all 13×13 pairs are allowed. */
    for (int s = 0; s < 169; ++s) cur[s] = 0;
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            cur[i * 13 + j] = 1;

    idx_t total_rows = n + 2;  /* height of the array */

    /* Extend the walk by adding rows 3 .. total_rows. */
    for (idx_t step = 3; step <= total_rows; ++step) {
        for (int s = 0; s < 169; ++s) nxt[s] = 0;  /* clear next counts */
        for (int s = 0; s < 169; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            int d = outdeg[s];
            for (int t = 0; t < d; ++t) {
                int to = adj[s][t];
                nxt[to] += val;
            }
        }
        for (int s = 0; s < 169; ++s) cur[s] = nxt[s];
    }

    /* Sum over all final state counts to obtain the answer. */
    seq_t result = 0;
    for (int s = 0; s < 169; ++s) result += cur[s];
    return result;
}
