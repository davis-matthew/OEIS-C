/*
 * A236786  Number of (n+1) X (5+1) 0..2 arrays with the upper median plus the
 *          lower median minus the minimum of every 2 X 2 subblock equal.
 * https://oeis.org/A236786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of (n+1)×6 arrays with entries 0..2 such
 *            that for every 2×2 subblock the value upper_median +
 *            lower_median – minimum is the same constant
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a graph of all 3^6=729 rows and
   counts walks of length n in that graph, summed over all possible
   constants (0..4). */

#include <stdbool.h>
#include <stdint.h>

#define STATE_COUNT 729          /* 3^6 */
#define MAX_NEIGHBOURS STATE_COUNT

/* ---- Precomputed tables ---- */
static unsigned char row_val[STATE_COUNT][6];            /* base‑3 digits of a row */
static uint16_t neigh[MAX_NEIGHBOURS][STATE_COUNT][MAX_NEIGHBOURS];
static unsigned short neigh_cnt[5][STATE_COUNT];
static bool built = false;

/* Computes the value upper_median + lower_median – minimum
   for the 2×2 block formed by the four given elements. */
// elements are 0,1,2
static int block_value(int a, int b, int c, int d)
{
    int v[4] = {a, b, c, d};
    int minv = 2, maxv = 0, sum = 0;
    for (int i = 0; i < 4; ++i) {
        int x = v[i];
        sum += x;
        if (x < minv) minv = x;
        if (x > maxv) maxv = x;
    }
    /* a2 + a3 - a1 = (sum - a1 - a4) - a1 = sum - a4 - 2*a1 */
    return sum - maxv - 2 * minv;   /* returns a value in 0..4 */
}

/* Build the row table and adjacency lists once. */
static void build_tables(void)
{
    if (built) return;
    built = true;

    /* 1. Generate all rows */
    for (int r = 0; r < STATE_COUNT; ++r) {
        int tmp = r;
        for (int j = 0; j < 6; ++j) {
            row_val[r][j] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }

    /* 2. Build adjacency lists for each constant C in 0..4 */
    for (int c = 0; c < 5; ++c)
        for (int r = 0; r < STATE_COUNT; ++r)
            neigh_cnt[c][r] = 0;

    for (int r = 0; r < STATE_COUNT; ++r) {
        for (int s = 0; s < STATE_COUNT; ++s) {
            int cand = -1;
            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                int val = block_value(row_val[r][j], row_val[r][j + 1],
                                     row_val[s][j], row_val[s][j + 1]);
                if (j == 0) cand = val;
                else if (val != cand) { ok = false; break; }
            }
            if (ok) {
                neigh[cand][r][neigh_cnt[cand][r]++] = (uint16_t)s;
            }
        }
    }
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    build_tables();

    seq_t result = (seq_t)0;   /* accumulation over all constants */

    for (int C = 0; C < 5; ++C) {
        static seq_t dp_cur[STATE_COUNT];
        static seq_t dp_next[STATE_COUNT];

        /* initialize for length 1 (single row) */
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = (seq_t)1;

        /* perform n transitions */
        for (idx_t step = 1; step <= n; ++step) {
            for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;
            for (int r = 0; r < STATE_COUNT; ++r) {
                seq_t val = dp_cur[r];
                if (val == (seq_t)0) continue;
                unsigned short cnt = neigh_cnt[C][r];
                uint16_t *list = neigh[C][r];
                for (unsigned short k = 0; k < cnt; ++k) {
                    int s = list[k];
                    dp_next[s] += val;
                }
            }
            /* swap dp_cur and dp_next */
            for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
        }

        /* sum for this constant */
        seq_t sumC = (seq_t)0;
        for (int i = 0; i < STATE_COUNT; ++i)
            sumC += dp_cur[i];
        result += sumC;
    }

    return result;
}
