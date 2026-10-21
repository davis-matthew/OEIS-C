/*
 * A208009  Number of n X 4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 78, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208009: Count of n×4 binary arrays avoiding 000,101 horizontally and 001,011 vertically.
   Computed via dynamic programming over pairs of consecutive rows. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static int rows[9];
static int trans[81][9];
static int transCount[81];

static void init_once(void)
{
    static bool done = false;
    if (done) return;
    /* build all 4‑bit rows that have no horizontal 000 or 101 */
    int cnt = 0;
    for (int r = 0; r < 16; ++r) {
        bool ok = true;
        for (int pos = 0; pos <= 1; ++pos) {
            int a = (r >> pos) & 1;
            int b = (r >> (pos + 1)) & 1;
            int c = (r >> (pos + 2)) & 1;
            if ((a == 0 && b == 0 && c == 0) ||
                (a == 1 && b == 0 && c == 1)) {
                ok = false;
                break;
            }
        }
        if (ok) rows[cnt++] = r;
    }
    /* build adjacency for transitions of state (prevRow, curRow) -> nextRow */
    for (int i = 0; i < 81; ++i) {
        int r1 = i / 9;
        int r2 = i % 9;
        int c = 0;
        for (int r3 = 0; r3 < 9; ++r3) {
            bool good = true;
            for (int col = 0; col < 4; ++col) {
                int a = (rows[r1] >> col) & 1;
                int b = (rows[r2] >> col) & 1;
                int d = (rows[r3] >> col) & 1;
                if ((a == 0 && b == 0 && d == 1) ||
                    (a == 0 && b == 1 && d == 1)) {
                    good = false;
                    break;
                }
            }
            if (good) trans[i][c++] = r2 * 9 + r3;
        }
        transCount[i] = c;
    }
    done = true;
}

seq_t a(idx_t n)
{
    init_once();

    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;   /* 9×9 */

    seq_t dp[81];
    seq_t next[81];

    /* sequences of length 2: one for each pair of rows */
    for (int i = 0; i < 81; ++i) dp[i] = (seq_t)1;

    for (idx_t row = 3; row <= n; ++row) {
        /* zero next */
        for (int i = 0; i < 81; ++i) next[i] = (seq_t)0;
        /* transition */
        for (int s = 0; s < 81; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            int tcnt = transCount[s];
            int *tlist = trans[s];
            for (int k = 0; k < tcnt; ++k) {
                int ns = tlist[k];
                next[ns] += val;
            }
        }
        /* copy next to dp */
        for (int i = 0; i < 81; ++i) dp[i] = next[i];
    }

    /* sum all sequences of length n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 81; ++i) total += dp[i];
    return total;
}
