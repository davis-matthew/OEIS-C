/*
 * A195957  Number of nX3 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors unequal to itself.
 * https://oeis.org/A195957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of n×3 arrays with entries 0..4 such that * each cell
 *            equals the number of horizontally and vertically adjacent *
 *            cells that differ from it. The sequence starts * 2, 6, 10, 16,
 *            21, 44, 82, 144, 273, 531, … (OEIS A195957)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

/* 125 possible rows (3 cells, each 0..4).  */
#define STATE_COUNT 125

/*  state vectors */
static int rows[STATE_COUNT][3];

/*  pre–computed tables */
static uint8_t init_valid[STATE_COUNT * STATE_COUNT];     /* prev valid while adding first row */
static uint8_t end_valid[STATE_COUNT * STATE_COUNT];     /* last row valid (no below neighbor) */
static uint16_t adjacency_start[STATE_COUNT * STATE_COUNT];
static uint8_t  adjacency_count[STATE_COUNT * STATE_COUNT];
static uint8_t  adjacency_pool[STATE_COUNT * STATE_COUNT * STATE_COUNT];

/*  dynamic programming buffers */
static seq_t dp_curr[STATE_COUNT * STATE_COUNT];
static seq_t dp_next[STATE_COUNT * STATE_COUNT];

/*  number of single‑row arrays satisfying the rule with no neighbours. */
static seq_t single_valid_count = 0;

static int tables_initialized = 0;

/*  check whether a row satisfies the rule given its above and below rows.
    above, below may be -1 to indicate absence.  */
static inline uint8_t row_valid(int row, int above, int below)
{
    for (int j = 0; j < 3; ++j) {
        int val = rows[row][j];
        int diff = 0;
        if (j > 0) diff += (val != rows[row][j - 1]);       /* left   */
        if (j < 2) diff += (val != rows[row][j + 1]);       /* right  */
        if (above >= 0) diff += (val != rows[above][j]);    /* above  */
        if (below >= 0) diff += (val != rows[below][j]);    /* below  */
        if (diff != val) return 0;
    }
    return 1;
}

/*  initialise all lookup tables – executed once.  */
static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = 1;

    /*  generate the 125 row vectors  */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int x = s;
        rows[s][0] = x % 5;  x /= 5;
        rows[s][1] = x % 5;  x /= 5;
        rows[s][2] = x % 5;
    }

    /*  single‑row validity count  */
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (row_valid(s, -1, -1)) single_valid_count += 1;
    }

    /*  first‑row transition validity  */
    for (int prev = 0; prev < STATE_COUNT; ++prev)
        for (int curr = 0; curr < STATE_COUNT; ++curr)
            init_valid[prev * STATE_COUNT + curr] =
                row_valid(prev, -1, curr);

    /*  last‑row final‑check validity  */
    for (int prev = 0; prev < STATE_COUNT; ++prev)
        for (int curr = 0; curr < STATE_COUNT; ++curr)
            end_valid[prev * STATE_COUNT + curr] =
                row_valid(curr, prev, -1);

    /*  build adjacency lists for middle rows  */
    size_t total = 0;
    for (int prev = 0; prev < STATE_COUNT; ++prev) {
        for (int curr = 0; curr < STATE_COUNT; ++curr) {
            size_t idx = (size_t)prev * STATE_COUNT + curr;
            adjacency_start[idx] = (uint16_t)total;
            adjacency_count[idx] = 0;
            for (int next = 0; next < STATE_COUNT; ++next) {
                if (row_valid(curr, prev, next)) {
                    adjacency_pool[total++] = (uint8_t)next;
                    adjacency_count[idx] += 1;
                }
            }
        }
    }
}

/*  OEIS sequence A195957  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for 0  */
    if (!tables_initialized) init_tables();

    if (n == 1) return single_valid_count;

    /*  initialise DP for 2 rows  */
    memset(dp_curr, 0, sizeof(dp_curr));
    for (size_t idx = 0; idx < STATE_COUNT * STATE_COUNT; ++idx)
        if (init_valid[idx]) dp_curr[idx] = (seq_t)1;

    /*  iterate for rows 3 .. n  */
    if (n >= 3) {
        for (idx_t step = 3; step <= n; ++step) {
            memset(dp_next, 0, sizeof(dp_next));
            for (size_t idx = 0; idx < STATE_COUNT * STATE_COUNT; ++idx) {
                seq_t cnt = dp_curr[idx];
                if (!cnt) continue;

                size_t start = adjacency_start[idx];
                uint8_t deg   = adjacency_count[idx];
                int    curr   = (int)(idx % STATE_COUNT);

                for (uint8_t t = 0; t < deg; ++t) {
                    uint8_t next = adjacency_pool[start + t];
                    dp_next[curr * STATE_COUNT + next] += cnt;
                }
            }
            memcpy(dp_curr, dp_next, sizeof(dp_curr));
        }
    }

    /*  final check for the bottom row  */
    seq_t result = 0;
    for (size_t idx = 0; idx < STATE_COUNT * STATE_COUNT; ++idx)
        if (end_valid[idx]) result += dp_curr[idx];

    return result;
}
