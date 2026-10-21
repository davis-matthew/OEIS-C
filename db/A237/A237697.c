/*
 * A237697  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of (n+1)×3 arrays with entries in {0,1,2}
   such that for every 2×2 subblock the sum of its maximum, upper median,
   and lower median differs from the sums of all horizontally and vertically
   adjacent subblocks by exactly one.  The computation is performed by
   dynamic programming on states that are pairs of consecutive rows. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* All rows are 3 digits in base 3; there are 3³ = 27 possible rows. */
    enum { ROWS = 27,        /* 3³ */
           MAX_STATES = 27 * 27 }; /* pair of rows */

    /* Static data that is initialized once. */
    static bool ready = false;
    static int state_id[ROWS][ROWS];    /* mapping (a,b) -> state index or -1 */
    static int prev_row[MAX_STATES];    /* a */
    static int next_row[MAX_STATES];    /* b */
    static int sum_left[MAX_STATES];    /* sum of left 2×2 block for the pair */
    static int sum_right[MAX_STATES];   /* sum of right 2×2 block for the pair */
    static int neigh[MAX_STATES][ROWS]; /* list of next states */
    static int neigh_cnt[MAX_STATES];   /* number of neighbours */
    static int rows_digits[ROWS][3];    /* row r: digits 0..2 */
    static int M = 0;                   /* number of valid states */

    if (!ready) {
        /* enumerate all rows */
        for (int r = 0; r < ROWS; ++r) {
            int x = r;
            for (int c = 0; c < 3; ++c) {
                rows_digits[r][c] = x % 3;
                x /= 3;
            }
        }
        /* build all valid states */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                /* values in the two 2×2 blocks */
                int v0[4] = {rows_digits[a][0], rows_digits[a][1],
                             rows_digits[b][0], rows_digits[b][1]};
                int v1[4] = {rows_digits[a][1], rows_digits[a][2],
                             rows_digits[b][1], rows_digits[b][2]};
                /* compute the correct minimum for each block */
                int min0 = v0[0];
                if (v0[1] < min0) min0 = v0[1];
                if (v0[2] < min0) min0 = v0[2];
                if (v0[3] < min0) min0 = v0[3];
                int min1 = v1[0];
                if (v1[1] < min1) min1 = v1[1];
                if (v1[2] < min1) min1 = v1[2];
                if (v1[3] < min1) min1 = v1[3];
                /* sum of the three largest values = total - minimum */
                int sum0 = v0[0] + v0[1] + v0[2] + v0[3] - min0;
                int sum1 = v1[0] + v1[1] + v1[2] + v1[3] - min1;
                if ( (sum0 > sum1 ? sum0 - sum1 : sum1 - sum0) == 1 ) {
                    state_id[a][b] = M;
                    prev_row[M] = a;
                    next_row[M] = b;
                    sum_left[M] = sum0;
                    sum_right[M] = sum1;
                    ++M;
                } else {
                    state_id[a][b] = -1;
                }
            }
        }
        /* build adjacency list for transitions (b,c) */
        for (int u = 0; u < M; ++u) {
            int b = next_row[u];
            int cnt = 0;
            for (int c = 0; c < ROWS; ++c) {
                int v = state_id[b][c];
                if (v != -1) {
                    int d0 = sum_left[u] - sum_left[v];
                    if (d0 < 0) d0 = -d0;
                    int d1 = sum_right[u] - sum_right[v];
                    if (d1 < 0) d1 = -d1;
                    if (d0 == 1 && d1 == 1) {
                        neigh[u][cnt++] = v;
                    }
                }
            }
            neigh_cnt[u] = cnt;
        }
        ready = true;
    }

    /* DP over number of transitions = n-1 */
    static seq_t dp_a[MAX_STATES];
    static seq_t dp_b[MAX_STATES];
    seq_t *cur = dp_a;
    seq_t *next = dp_b;

    /* initialise: every first state counts as one */
    for (int i = 0; i < M; ++i) cur[i] = (seq_t)1;
    for (int i = 0; i < M; ++i) next[i] = (seq_t)0;

    for (int step = 1; step < (int)n; ++step) {    /* n may be > int: cast is safe since n≥1 but n≤10000 */
        /* zero next */
        for (int i = 0; i < M; ++i) next[i] = (seq_t)0;
        for (int u = 0; u < M; ++u) {
            seq_t val = cur[u];
            if (val == 0) continue;
            int cnt = neigh_cnt[u];
            for (int k = 0; k < cnt; ++k) {
                int v = neigh[u][k];
                next[v] += val;
            }
        }
        /* swap */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all final counts */
    seq_t result = (seq_t)0;
    for (int i = 0; i < M; ++i) result += cur[i];
    return result;
}
