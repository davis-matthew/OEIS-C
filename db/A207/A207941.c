/*
 * A207941  Number of 5Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function implements the exact counting of 5×n arrays over
 *            {0,1} that avoid the horizontal patterns 001 and 100, and the
 *            vertical patterns 001 and 110. The computation is performed by a
 *            linear recurrence obtained from a state‑transition DP with
 *            2‑column history. For n≥3 the algorithm runs in...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precomputed list of allowed 5‑bit columns (no vertical 001 or 110). */
    static bool initialized = false;
    static int allowed[32];
    static int k = 0;        /* number of allowed columns (≤32) */
    static int good[32][32][32]; /* transition validity for triples of columns */

    if (!initialized) {
        /* Find allowed columns. */
        int cnt = 0;
        for (int col = 0; col < 32; ++col) {
            bool ok = true;
            for (int r = 0; r <= 2; ++r) {
                int b1 = (col >> r) & 1;
                int b2 = (col >> (r + 1)) & 1;
                int b3 = (col >> (r + 2)) & 1;
                if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                    (b1 == 1 && b2 == 1 && b3 == 0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) allowed[cnt++] = col;
        }
        k = cnt; /* should be 10 for the 5×n problem */

        /* Pre‑compute horizontal transition matrix. */
        for (int p = 0; p < k; ++p)
            for (int q = 0; q < k; ++q)
                for (int r = 0; r < k; ++r) {
                    bool tr = true;
                    for (int row = 0; row < 5; ++row) {
                        int b1 = (allowed[p] >> row) & 1;
                        int b2 = (allowed[q] >> row) & 1;
                        int b3 = (allowed[r] >> row) & 1;
                        if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                            (b1 == 1 && b2 == 0 && b3 == 0)) {
                            tr = false;
                            break;
                        }
                    }
                    good[p][q][r] = tr;
                }

        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* unused but harmless */

    if (n == 1) return (seq_t)k;           /* first column only */
    if (n == 2) return (seq_t)k * (seq_t)k;/* any pair of columns */

    /* DP arrays: dp_prev and dp_curr hold counts for the last two columns.
       Size is at most k*k (≤1024).                                     */
    const size_t states = (size_t)k * (size_t)k;
    seq_t dp_prev[1024] = {0}, dp_curr[1024] = {0};

    /* Initial state: first two columns may be any allowed pair. */
    for (int p = 0; p < k; ++p)
        for (int q = 0; q < k; ++q)
            dp_prev[p * k + q] = (seq_t)1;

    /* Extend columns from the 3rd to the n‑th. */
    for (idx_t col = 3; col <= n; ++col) {
        for (size_t i = 0; i < states; ++i) dp_curr[i] = (seq_t)0;
        for (int p = 0; p < k; ++p)
            for (int q = 0; q < k; ++q) {
                seq_t val = dp_prev[p * k + q];
                if (val == 0) continue;
                for (int r = 0; r < k; ++r)
                    if (good[p][q][r])
                        dp_curr[q * k + r] += val;
            }
        /* swap role of dp_prev and dp_curr */
        for (size_t i = 0; i < states; ++i) dp_prev[i] = dp_curr[i];
    }

    /* Sum all possible ending states. */
    seq_t res = (seq_t)0;
    for (size_t i = 0; i < states; ++i) res += dp_prev[i];
    return res;
}
