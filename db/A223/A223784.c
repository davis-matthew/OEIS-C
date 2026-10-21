/*
 * A223784  Number of n X 3 0..2 arrays with rows, diagonals and antidiagonals
 *          unimodal.
 * https://oeis.org/A223784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A223784: number of n×3 arrays with entries 0–2 such that
   every row, every main diagonal, and every anti‑diagonal is unimodal.
   The algorithm expands the array row by row.  For each new row the last
   two rows are kept in the state; a transition to the new row is
   accepted only if it together with the two preceding rows keeps all
   required diagonals unimodal.  All calculations use the type seq_t
   supplied by the build.  */

#include <stddef.h>
#include <stdbool.h>

#define STATES 27          /* 3^3 possible rows (0..2) */
#define PAIRS (STATES*STATES)

static unsigned char row_vals[STATES][3];
static unsigned char row_unimodal[STATES];
static unsigned char next_len[PAIRS];
static unsigned char next_rows[PAIRS][STATES];
static bool init_done = false;

static void init_tables(void)
{
    if (init_done) return;
    /* Enumerate all 3‑tuples and test unimodality of a row */
    for (int idx = 0; idx < STATES; ++idx) {
        int v = idx;
        row_vals[idx][0] = v % 3; v /= 3;
        row_vals[idx][1] = v % 3; v /= 3;
        row_vals[idx][2] = v % 3;
        int a = row_vals[idx][0];
        int b = row_vals[idx][1];
        int c = row_vals[idx][2];
        /* a row is unimodal iff (a>=b>=c) || (a<=b>=c) || (a<=b<=c) */
        row_unimodal[idx] =
            (a>=b && b>=c) || (a<=b && b>=c) || (a<=b && b<=c);
    }

    /* Pre‑compute transitions: for each pair (prevprev, prev)
       list all next rows that keep the two length‑3 diagonals unimodal */
    for (int p = 0; p < STATES; ++p) {
        for (int q = 0; q < STATES; ++q) {
            int pos = p * STATES + q;
            if (!row_unimodal[p] || !row_unimodal[q]) {
                next_len[pos] = 0;
                continue;
            }
            int count = 0;
            for (int r = 0; r < STATES; ++r) {
                if (!row_unimodal[r]) continue;
                int a0 = row_vals[p][0];
                int a1 = row_vals[p][1];
                int a2 = row_vals[p][2];
                int b0 = row_vals[q][0];
                int b1 = row_vals[q][1];
                int b2 = row_vals[q][2];
                int c0 = row_vals[r][0];
                int c1 = row_vals[r][1];
                int c2 = row_vals[r][2];

                /* diagonal NW–SE: (p[0], q[1], r[2]) */
                int d1 = a0, d2 = b1, d3 = c2;
                int ok1 = (d1>=d2 && d2>=d3) ||
                          (d1<=d2 && d2>=d3) ||
                          (d1<=d2 && d2<=d3);
                /* diagonal NE–SW: (p[2], q[1], r[0]) */
                int e1 = a2, e2 = b1, e3 = c0;
                int ok2 = (e1>=e2 && e2>=e3) ||
                          (e1<=e2 && e2>=e3) ||
                          (e1<=e2 && e2<=e3);

                if (ok1 && ok2) {
                    next_rows[pos][count++] = (unsigned char)r;
                }
            }
            next_len[pos] = count;
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    if (n == 1) return (seq_t)22;          /* a(1) */
    if (n == 2) return (seq_t)484;         /* a(2) */

    seq_t dp_cur[PAIRS];
    seq_t dp_next[PAIRS];

    /* initialise dp for the first two rows */
    for (int i = 0; i < PAIRS; ++i) dp_cur[i] = (seq_t)0;
    for (int p = 0; p < STATES; ++p) {
        if (!row_unimodal[p]) continue;
        for (int q = 0; q < STATES; ++q) {
            if (!row_unimodal[q]) continue;
            dp_cur[p*STATES + q] = (seq_t)1;
        }
    }

    /* add rows 3 … n */
    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < PAIRS; ++i) dp_next[i] = (seq_t)0;
        for (int p = 0; p < STATES; ++p) {
            for (int q = 0; q < STATES; ++q) {
                int pos = p*STATES + q;
                seq_t val = dp_cur[pos];
                if (val == 0) continue;
                unsigned char *c_list = next_rows[pos];
                unsigned char c_cnt   = next_len[pos];
                for (int k = 0; k < c_cnt; ++k) {
                    int r = c_list[k];
                    dp_next[q*STATES + r] += val;
                }
            }
        }
        /* move to next iteration */
        for (int i = 0; i < PAIRS; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all ending configurations */
    seq_t result = (seq_t)0;
    for (int i = 0; i < PAIRS; ++i) result += dp_cur[i];
    return result;
}
