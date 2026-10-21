/*
 * A208037  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays avoiding the * horizontal
 *            patterns 0 0 0 and 0 1 0 in every row, and the * vertical
 *            patterns 0 0 0 and 1 0 1 in every column. * The values are
 *            computed by dynamic programming over the * admissible 6‑bit
 *            rows. No explicit recurrence is needed
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_ROWS 64   /* 2^6 */

static bool init_done = false;
static uint8_t rows[MAX_ROWS];
static int    row_count = 0;
static unsigned char trans[MAX_ROWS][MAX_ROWS][MAX_ROWS];

/* Compute the admissible rows and the vertical transition table. */
static void
init_automaton(void)
{
    if (init_done) return;
    init_done = true;

    /* Find all 6‑bit rows that avoid 0 0 0 and 0 1 0 horizontally. */
    row_count = 0;
    for (int r = 0; r < 64; ++r) {
        bool ok = true;
        for (int i = 0; i <= 3; ++i) {
            int p = (r >> i)     & 1;
            int q = (r >> (i+1)) & 1;
            int t = (r >> (i+2)) & 1;
            if ((p == 0 && q == 0 && t == 0) ||
                (p == 0 && q == 1 && t == 0))
            { ok = false; break; }
        }
        if (ok) rows[row_count++] = (uint8_t)r;
    }

    /* Precompute vertical transitions:
     * trans[a][b][c] == 1 if rows a,b,c can appear consecutively
     * without creating forbidden vertical 3‑tuples.  Forbidden
     * patterns are 0 0 0 and 1 0 1. */
    for (int a = 0; a < row_count; ++a)
        for (int b = 0; b < row_count; ++b) {
            uint8_t ra = rows[a];
            uint8_t rb = rows[b];
            for (int c = 0; c < row_count; ++c) {
                uint8_t rc = rows[c];
                bool ok = true;
                for (int col = 0; col < 6; ++col) {
                    int p = (ra >> col) & 1;
                    int q = (rb >> col) & 1;
                    int t = (rc >> col) & 1;
                    if ((p == 0 && q == 0 && t == 0) ||
                        (p == 1 && q == 0 && t == 1)) {
                        ok = false;
                        break;
                    }
                }
                trans[a][b][c] = ok ? 1 : 0;
            }
        }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* out of range (offset is 1) */

    init_automaton();

    if (n == 1) return (seq_t)row_count;
    if (n == 2) return (seq_t)row_count * (seq_t)row_count;

    /* DP over rows: dpPrev[a][b] = number of ways to end with rows a,b. */
    static seq_t dpPrev[MAX_ROWS][MAX_ROWS];
    static seq_t dpCurr[MAX_ROWS][MAX_ROWS];

    /* Initialize for n = 2 (any pair of valid rows). */
    for (int a = 0; a < row_count; ++a)
        for (int b = 0; b < row_count; ++b)
            dpPrev[a][b] = 1;

    for (idx_t step = 3; step <= n; ++step) {
        /* clear current DP */
        for (int a = 0; a < row_count; ++a)
            for (int b = 0; b < row_count; ++b)
                dpCurr[a][b] = 0;

        /* transition: for every previous two rows (a,b) add all
           allowed third rows c. */
        for (int a = 0; a < row_count; ++a)
            for (int b = 0; b < row_count; ++b) {
                seq_t val = dpPrev[a][b];
                if (val == 0) continue;
                for (int c = 0; c < row_count; ++c)
                    if (trans[a][b][c])
                        dpCurr[b][c] += val;
            }

        /* move dpCurr to dpPrev for next step */
        for (int a = 0; a < row_count; ++a)
            for (int b = 0; b < row_count; ++b)
                dpPrev[a][b] = dpCurr[a][b];
    }

    /* Sum all end states to obtain the total number of arrays. */
    seq_t result = 0;
    for (int a = 0; a < row_count; ++a)
        for (int b = 0; b < row_count; ++b)
            result += dpPrev[a][b];

    return result;
}
