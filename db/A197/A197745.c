/*
 * A197745  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,3,0,1,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 3 0..4 arrays such that each entry x has *
 *            exactly t(x) neighboring (horizontal or vertical) entries equal
 *            to 3, * where t = {3,3,0,1,0} for x = {0,1,2,3,4}. The
 *            computation is * performed by a transfer‑matrix DP over pairs of
 *            consecutive rows
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ROWS 125   /* 5^3 possible rows for width 3 */
#define COLS 3

/* Target numbers for each cell value */
static const uint8_t target_map[5] = {3, 3, 0, 1, 0};

/* Storage for precomputed tables */
static bool tables_initialized = false;
static uint8_t values[ROWS][COLS];
static bool single_ok[ROWS];
static uint8_t next_cnt[ROWS][ROWS];
static uint8_t next_list[ROWS][ROWS][ROWS];  /* up to ROWS possible nexts */

/* Helper that checks whether a single row satisfies the conditions
 * when optional up/down rows are provided. */
static bool row_valid(const uint8_t *row,
                      const uint8_t *up,  bool has_up,
                      const uint8_t *down, bool has_down)
{
    for (int c = 0; c < COLS; c++) {
        uint8_t x = row[c];
        uint8_t t = target_map[x];
        int cnt = 0;
        if (c > 0   && row[c-1] == t) cnt++;
        if (c < COLS-1 && row[c+1] == t) cnt++;
        if (has_up   && up[c]   == t) cnt++;
        if (has_down && down[c] == t) cnt++;
        if (cnt != x) return false;
    }
    return true;
}

/* Fill the tables the first time the function is called. */
static void init_tables(void)
{
    if (tables_initialized) return;
    /* Enumerate all row patterns. */
    for (int id = 0; id < ROWS; id++) {
        int r = id;
        for (int c = 0; c < COLS; c++) {
            values[id][c] = r % 5;
            r /= 5;
        }
    }
    /* Rows valid when alone (no up, no down). */
    for (int i = 0; i < ROWS; i++)
        single_ok[i] = row_valid(values[i], NULL, false, NULL, false);
    /* For each pair (prev,cur) precompute list of next rows that keep
     * cur row valid (with up=prev, down=next). */
    for (int prev = 0; prev < ROWS; prev++) {
        for (int cur = 0; cur < ROWS; cur++) {
            uint8_t cnt = 0;
            for (int next = 0; next < ROWS; next++) {
                if (row_valid(values[cur], values[prev], true,
                              values[next], true)) {
                    next_list[prev][cur][cnt++] = next;
                }
            }
            next_cnt[prev][cur] = cnt;
        }
    }
    tables_initialized = true;
}

/* Main sequence function.  It uses a DP that progresses row by row
 * building up the number of completions that agree with the
 * transfer‑matrix constraints.  The function caches intermediate
 * results so that calls in arbitrary order are efficient. */
seq_t a(idx_t n)
{
    init_tables();

    /* Base case n == 1: just count rows that satisfy the condition
     * by themselves. */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < ROWS; r++)
            if (single_ok[r]) ans += (seq_t)1;
        return ans;
    }

    /* DP arrays (static so they reuse allocations across calls). */
    static seq_t dp_prev[ROWS][ROWS];
    static seq_t dp_cur [ROWS][ROWS];

    /* Clear dp_prev. */
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < ROWS; j++)
            dp_prev[i][j] = (seq_t)0;

    /* Initialise for the first two rows. */
    for (int prev = 0; prev < ROWS; prev++)
        for (int cur = 0; cur < ROWS; cur++)
            /* A valid first row is required to have no neighbours
             * up, so we keep all pairs where the first row is
             * valid with up absent and down = cur. */
            if (row_valid(values[prev], NULL, false, values[cur], true))
                dp_prev[prev][cur] = (seq_t)1;

    /* Process rows 3 .. n. */
    for (idx_t row = 3; row <= n; row++) {
        /* Clear current DP. */
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < ROWS; j++)
                dp_cur[i][j] = (seq_t)0;

        /* Transition from every state (prev,cur) to possible nexts. */
        for (int prev = 0; prev < ROWS; prev++) {
            for (int cur = 0; cur < ROWS; cur++) {
                seq_t ways = dp_prev[prev][cur];
                if (!ways) continue;
                uint8_t cnt = next_cnt[prev][cur];
                for (uint8_t k = 0; k < cnt; k++) {
                    int nxt = next_list[prev][cur][k];
                    dp_cur[cur][nxt] += ways;
                }
            }
        }
        /* Swap dp_prev and dp_cur for next iteration. */
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < ROWS; j++)
                dp_prev[i][j] = dp_cur[i][j];
    }

    /* Sum over states that leave a valid final row
     * (cur has no down neighbour). */
    seq_t ans = 0;
    for (int prev = 0; prev < ROWS; prev++) {
        for (int cur = 0; cur < ROWS; cur++) {
            if (row_valid(values[cur], values[prev], true, NULL, false))
                ans += dp_prev[prev][cur];
        }
    }
    return ans;
}
