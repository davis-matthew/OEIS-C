/*
 * A207705  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   valid_horizontal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207705: Number of 4×n binary arrays
   avoiding horizontally 000 and 010,
   and vertically 001 and 110.
   The algorithm counts walks in a state graph
   whose nodes are ordered pairs of admissible
   column patterns.                                   */
#include <stdbool.h>
#include "../utils/valid_horizontal.h"

static inline bool valid_vertical(int col)
{
    // check two triples: rows 0-2 and 1-3 (top to bottom)
    for (int r = 0; r < 2; ++r) {
        int b0 = (col >> r)     & 1;
        int b1 = (col >> (r+1)) & 1;
        int b2 = (col >> (r+2)) & 1;
        if ((b0 == 0 && b1 == 0 && b2 == 1) ||
            (b0 == 1 && b1 == 1 && b2 == 0))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* offset starts at 1 */

    /* admissible 4‑bit columns */
    int cols[16];
    int cc = 0;
    for (int col = 0; col < 16; ++col)
        if (valid_vertical(col))
            cols[cc++] = col;

    /* n == 1: only one column */
    if (n == 1)
        return (seq_t)cc;

    /* state is an ordered pair (prev, curr) of columns */
    const int MAX_C = 16;
    const int MAX_S = MAX_C * MAX_C;   /* max 256 states */

    seq_t dp[MAX_S];
    seq_t next[MAX_S];

    /* initialize for length 2 */
    for (int i = 0; i < MAX_S; ++i) dp[i] = 0;
    for (int p = 0; p < cc; ++p)
        for (int q = 0; q < cc; ++q)
            dp[p * cc + q] = 1;

    /* iterate from length 2 up to n-1 (building sequences of length >2) */
    for (idx_t len = 2; len < n; ++len) {
        for (int i = 0; i < cc * cc; ++i) next[i] = 0;
        for (int id = 0; id < cc * cc; ++id) {
            seq_t cur = dp[id];
            if (cur == 0) continue;
            int p = id / cc;             /* previous column */
            int q = id % cc;             /* current column */
            int colP = cols[p];
            int colQ = cols[q];
            for (int r = 0; r < cc; ++r) {
                int colR = cols[r];
                if (valid_horizontal(colP, colQ, colR)) {
                    int nid = q * cc + r;   /* shift window */
                    next[nid] = next[nid] + cur;
                }
            }
        }
        for (int i = 0; i < cc * cc; ++i) dp[i] = next[i];
    }

    /* total count for length n */
    seq_t total = 0;
    for (int i = 0; i < cc * cc; ++i)
        total = total + dp[i];
    return total;
}
