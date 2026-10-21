/*
 * A202585  Number of (n+2)X4 binary arrays avoiding patterns 001 and 100 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic‑programming computation of sequence A202585.
   We count (n+2)×4 binary arrays avoiding the 3‑bit patterns 001
   and 100 in all rows, columns, and NW→SE diagonals.
   No closed formula is known; the DP processes one row at a time
   while keeping the last two rows as state.  */

/* Precomputed validity tables: a row is valid if it has no 001/100
   horizontally, and allowed_next[p<<4|q][r] is true iff appending row
   r after rows p and q produces no 001/100 vertically or diagonally. */

#include <stdbool.h>
#include <stddef.h>

static bool valid_row[16];
static bool allowed_next[256][16];
static bool init_done = false;

static void init_validity(void)
{
    if (init_done) return;
    /* Rows that contain no 001 or 100 horizontally. */
    for (int r = 0; r < 16; ++r) {
        bool ok = true;
        for (int j = 0; j <= 1; ++j) {
            int tri = (r >> j) & 7;          /* 3 consecutive bits */
            if (tri == 1 || tri == 4) {       /* 001 or 100 */
                ok = false;
                break;
            }
        }
        valid_row[r] = ok;
    }

    /* Transition table for adding a row after two previous rows. */
    for (int p = 0; p < 16; ++p) if (valid_row[p]) {
        for (int q = 0; q < 16; ++q) if (valid_row[q]) {
            int state = (p << 4) | q;        /* high nibble p, low nibble q */
            for (int cur = 0; cur < 16; ++cur) if (valid_row[cur]) {
                bool bad = false;
                /* vertical triple check for each column */
                for (int c = 0; c < 4 && !bad; ++c) {
                    int tri = ((p >> c) & 1) << 2 |
                              ((q >> c) & 1) << 1 |
                              ((cur >> c) & 1);
                    if (tri == 1 || tri == 4) bad = true;
                }
                /* NW→SE diagonal check (columns 0–1, rows i-2,i-1,i) */
                for (int c = 2; c < 4 && !bad; ++c) {
                    int tri = ((p >> (c - 2)) & 1) << 2 |
                              ((q >> (c - 1)) & 1) << 1 |
                              ((cur >> c) & 1);
                    if (tri == 1 || tri == 4) bad = true;
                }
                if (!bad) allowed_next[state][cur] = true;
            }
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n<1 not used */
    init_validity();

    idx_t rows = n + 2;            /* total number of rows */

    static seq_t dp[256];
    static seq_t dp_next[256];

    /* zero DP tables */
    for (int i = 0; i < 256; ++i) dp[i] = 0;

    /* initial two rows */
    for (int r0 = 0; r0 < 16; ++r0) if (valid_row[r0]) {
        for (int r1 = 0; r1 < 16; ++r1) if (valid_row[r1]) {
            int state = (r0 << 4) | r1;
            dp[state] = 1;
        }
    }

    /* iterate over remaining rows */
    for (idx_t step = 2; step < rows; ++step) {
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;
        for (int state = 0; state < 256; ++state) {
            seq_t cnt = dp[state];
            if (cnt == 0) continue;
            for (int cur = 0; cur < 16; ++cur) {
                if (!allowed_next[state][cur]) continue;
                int newstate = ((state & 0x0F) << 4) | cur;   /* shift only low nibble */
                dp_next[newstate] = dp_next[newstate] + cnt;
            }
        }
        /* copy dp_next into dp for next iteration */
        for (int i = 0; i < 256; ++i) dp[i] = dp_next[i];
    }

    /* sum all sequences ending in any pair of last rows */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total = total + dp[i];
    return total;
}
