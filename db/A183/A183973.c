/*
 * A183973  Half the number of (n+1)X3 0..3 arrays with no 2X2 subblock sum
 *          differing from a horizontal or vertical neighbor subblock sum by
 *          more than one.
 * https://oeis.org/A183973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) equals half the number of (n+1)×3 arrays over {0,1,2,3}
 * such that the sum of any 2×2 subblock differs by at most 1 from the
 * sum of any horizontally or vertically adjacent 2×2 subblock.
 * This implementation builds a DP over row states and returns a(n).
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* The maximum possible sum of a 2×2 block is 12 (3+3+3+3). */
#define MAX_SUM   12
#define MAX_ROWS  64          /* 2^6 possible rows of length 3 with digits 0..3 */

#define STATES_PER_ROW 37     /* number of (s0,s1) pairs for a given row */
#define STATES (STATES_PER_ROW * MAX_ROWS)

/* Pre‑computed data structures */
static int   row_vals[MAX_ROWS][3];
static bool  pair_ok[MAX_ROWS][MAX_ROWS];
static int   sum0[MAX_ROWS][MAX_ROWS];
static int   sum1[MAX_ROWS][MAX_ROWS];
static int   offset_prefix[MAX_SUM + 2];
static int   state_row[STATES];
static int   state_s0[STATES];
static int   state_s1[STATES];
static int   trans_arr[STATES][MAX_ROWS];
static size_t trans_cnt[STATES];

static seq_t dp0[STATES];
static seq_t dp1[STATES];

static bool  initialized = false;

/* ------------------------------------------------------------------ */
/* Helper: map a state (current_row, s0, s1) to a unique index. */
static int state_index(int row, int s0, int s1)
{
    int lower = (s0 == 0 ? 0 : s0 - 1);
    int offset = s1 - lower;        /* 0 .. (upper - lower) */
    return row * STATES_PER_ROW + offset_prefix[s0] + offset;
}

/* ------------------------------------------------------------------ */
/* Build all tables once. */
static void init(void)
{
    if (initialized) return;
    /* Pre‑compute row values. */
    for (int r = 0; r < MAX_ROWS; ++r) {
        for (int c = 0; c < 3; ++c) row_vals[r][c] = (r >> (2 * c)) & 3;
    }

    /* Sums and horizontal feasibility for each pair of rows. */
    for (int a = 0; a < MAX_ROWS; ++a) {
        for (int b = 0; b < MAX_ROWS; ++b) {
            int s0 = row_vals[a][0] + row_vals[a][1] +
                     row_vals[b][0] + row_vals[b][1];
            int s1 = row_vals[a][1] + row_vals[a][2] +
                     row_vals[b][1] + row_vals[b][2];
            sum0[a][b] = s0;
            sum1[a][b] = s1;
            pair_ok[a][b] = (abs(s0 - s1) <= 1);
        }
    }

    /* Prefix counts for (s0,s1) enumeration. */
    offset_prefix[0] = 0;
    for (int s0 = 0; s0 <= MAX_SUM; ++s0) {
        int lower = (s0 == 0 ? 0 : s0 - 1);
        int upper = (s0 == MAX_SUM ? MAX_SUM : s0 + 1);
        int count = upper - lower + 1;
        offset_prefix[s0 + 1] = offset_prefix[s0] + count;
    }

    /* Enumerate all states. */
    int idx = 0;
    for (int r = 0; r < MAX_ROWS; ++r) {
        for (int s0 = 0; s0 <= MAX_SUM; ++s0) {
            int lower = (s0 == 0 ? 0 : s0 - 1);
            int upper = (s0 == MAX_SUM ? MAX_SUM : s0 + 1);
            for (int s1 = lower; s1 <= upper; ++s1) {
                state_row[idx] = r;
                state_s0[idx]   = s0;
                state_s1[idx]   = s1;
                ++idx;
            }
        }
    }

    /* Pre‑compute valid transitions. */
    for (int st = 0; st < STATES; ++st) {
        trans_cnt[st] = 0;
        int r  = state_row[st];
        int s0 = state_s0[st];
        int s1 = state_s1[st];
        for (int nr = 0; nr < MAX_ROWS; ++nr) {
            if (!pair_ok[r][nr]) continue;              /* horizontal neighbour OK */
            int ns0 = sum0[r][nr];
            int ns1 = sum1[r][nr];
            int ds0 = ns0 - s0; if (ds0 < 0) ds0 = -ds0;
            int ds1 = ns1 - s1; if (ds1 < 0) ds1 = -ds1;
            if (ds0 <= 1 && ds1 <= 1) {                 /* vertical neighbour OK */
                int ns = state_index(nr, ns0, ns1);
                trans_arr[st][trans_cnt[st]++] = ns;
            }
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    init();

    /* Initialise DP tables to zero. */
    for (int i = 0; i < STATES; ++i) dp0[i] = 0;
    seq_t *dp_cur = dp0;
    seq_t *dp_next = dp1;

    /* Initialisation for the first two rows (n = 1). */
    for (int r0 = 0; r0 < MAX_ROWS; ++r0) {
        for (int r1 = 0; r1 < MAX_ROWS; ++r1) {
            if (!pair_ok[r0][r1]) continue;
            int s0 = sum0[r0][r1];
            int s1 = sum1[r0][r1];
            int idx = state_index(r1, s0, s1);
            dp_cur[idx] += 1;
        }
    }

    if (n == 1) {
        seq_t res = 0;
        for (int i = 0; i < STATES; ++i) res += dp_cur[i];
        return res / 2;                     /* halve the total count */
    }

    idx_t steps = n - 1;                    /* new rows to add */

    for (idx_t step = 0; step < steps; ++step) {
        /* Clear next DP array. */
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        /* Transition from current states to next states. */
        for (int st = 0; st < STATES; ++st) {
            seq_t val = dp_cur[st];
            if (val == 0) continue;
            size_t cnt = trans_cnt[st];
            for (size_t t = 0; t < cnt; ++t) {
                int ns = trans_arr[st][t];
                dp_next[ns] += val;
            }
        }

        /* Swap current and next arrays. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all final states and divide by 2. */
    seq_t res = 0;
    for (int i = 0; i < STATES; ++i) res += dp_cur[i];
    return res / 2;
}
