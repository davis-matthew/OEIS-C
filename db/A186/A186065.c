/*
 * A186065  Number of (n+2)X4 0..2 arrays with each 3X3 subblock having a
 *          positive determinant.
 * https://oeis.org/A186065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 10, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Sequence a(n): number of (n+2)×4 matrices over {0,1,2} such that every
   3×3 consecutive submatrix (columns 0–2 and columns 1–3) has a positive determinant. */

#define NUM_ROWS    81                 /* 3⁴ possible rows */
#define STATE_COUNT (NUM_ROWS * NUM_ROWS)

/* Storage for the 81 rows, transition tables and DP arrays */
static uint8_t  row_val[NUM_ROWS][4];
static int     next_rows[STATE_COUNT][NUM_ROWS];
static uint8_t  next_cnt[STATE_COUNT];
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
static int    init_done = 0;

/* 3×3 determinant of integer entries */
static inline int det3(int a11, int a12, int a13,
                       int a21, int a22, int a23,
                       int a31, int a32, int a33)
{
    return a11 * (a22 * a33 - a23 * a32)
         - a12 * (a21 * a33 - a23 * a31)
         + a13 * (a21 * a32 - a22 * a31);
}

/* Return 1 if rows p,q,r can appear consecutively (positive determinants) */
static inline int triple_ok(int p, int q, int r)
{
    const uint8_t *r1 = row_val[p];
    const uint8_t *r2 = row_val[q];
    const uint8_t *r3 = row_val[r];

    if (det3((int)r1[0], (int)r1[1], (int)r1[2],
             (int)r2[0], (int)r2[1], (int)r2[2],
             (int)r3[0], (int)r3[1], (int)r3[2]) <= 0)
        return 0;
    if (det3((int)r1[1], (int)r1[2], (int)r1[3],
             (int)r2[1], (int)r2[2], (int)r2[3],
             (int)r3[1], (int)r3[2], (int)r3[3]) <= 0)
        return 0;
    return 1;
}

/* One‑time precomputation of rows and transitions */
static void precompute(void)
{
    /* Generate all 81 rows as base‑3 vectors of length 4 */
    for (int idx = 0; idx < NUM_ROWS; ++idx) {
        int tmp = idx;
        for (int i = 3; i >= 0; --i) {
            row_val[idx][i] = tmp % 3;
            tmp /= 3;
        }
    }

    /* Build transition lists: for each pair (p,q) list of r that can follow */
    for (int p = 0; p < NUM_ROWS; ++p) {
        for (int q = 0; q < NUM_ROWS; ++q) {
            int state = p * NUM_ROWS + q;
            int cnt   = 0;
            for (int r = 0; r < NUM_ROWS; ++r) {
                if (triple_ok(p, q, r))
                    next_rows[state][cnt++] = r;
            }
            next_cnt[state] = (uint8_t)cnt;
        }
    }

    /* Allocate DP tables */
    dp_cur  = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    dp_next = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
}

seq_t a(idx_t n)
{
    if (!init_done) {
        precompute();
        init_done = 1;
    }

    /* Initialize DP for arrays of length 2 (any pair of rows allowed) */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp_cur[i] = 1;

    /* Add n rows one by one, each step increases height by 1 */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next DP */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;

            int q   = s % NUM_ROWS;
            int cnt = next_cnt[s];
            int *list = next_rows[s];
            for (int j = 0; j < cnt; ++j) {
                int r = list[j];
                int dest = q * NUM_ROWS + r;
                dp_next[dest] += val;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all completed arrays of height n+2 */
    seq_t result = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        result += dp_cur[i];

    return result;
}
