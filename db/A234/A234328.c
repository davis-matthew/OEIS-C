/*
 * A234328  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×4 arrays over {0,…,3} in which *
 *            every 2×2 subblock has its diagonal sum differing from its *
 *            anti‑diagonal sum by exactly 1. Computed by dynamic programming
 *            * over the 256 possible row states
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define ROWS 256
#define MAX_EDGES (ROWS * ROWS)

/* Tables (initialised once on first call) */
static uint8_t  row_val[ROWS][4];          /* decoded row, base‑4 digits   */
static uint16_t edge_start[ROWS];         /* first index of adjacency    */
static uint16_t edge_count[ROWS];         /* number of outgoing edges    */
static uint16_t edge_list[MAX_EDGES];     /* adjacency list             */
static bool     tables_ready = false;

/* ------------------------------------------------------------------ */
/* Helper: test whether two rows form a valid 2×2 block at all 3
 * horizontal positions.  For the rows r and c, we check that
 * |(r[j] + c[j+1]) − (r[j+1] + c[j])| == 1 for j = 0,1,2.        */
static inline bool valid_pair(int r, int c)
{
    const uint8_t *R = row_val[r];
    const uint8_t *C = row_val[c];
    for (int j = 0; j < 3; ++j) {
        int diff = (int)(R[j] + C[j+1]) - (int)(R[j+1] + C[j]);
        if (diff != 1 && diff != -1) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Initialise tables on first use.  Must be called before any use of
 * the tables.  All tables are statically allocated, so there is
 * no variable‑length array at file scope.                                */
static void init_tables(void)
{
    if (tables_ready) return;
    tables_ready = true;

    /* Decode all 256 rows into 4 base‑4 digits. */
    for (int r = 0; r < ROWS; ++r) {
        uint8_t v = (uint8_t)r;
        for (int k = 0; k < 4; ++k) {
            row_val[r][k] = v & 3;
            v >>= 2;
        }
    }

    /* Build adjacency lists. */
    int ptr = 0;
    for (int i = 0; i < ROWS; ++i) {
        edge_start[i] = ptr;
        int cnt = 0;
        for (int j = 0; j < ROWS; ++j) {
            if (valid_pair(i, j)) {
                edge_list[ptr++] = (uint16_t)j;
                ++cnt;
            }
        }
        edge_count[i] = (uint16_t)cnt;
    }
}

/* ------------------------------------------------------------------ */
/* Sequence entry point.  Indexed from 1 (a(1) = 2368).                */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset safety */
    init_tables();

    /* DP state: number of ways to reach each row after current step. */
    seq_t dp[ROWS];
    seq_t nxt[ROWS];
    for (int i = 0; i < ROWS; ++i) dp[i] = 1;   /* first row */

    /* Each extra row adds one transition.  There are n transitions. */
    for (idx_t step = 0; step < n; ++step) {
        /* clear next vector */
        for (int i = 0; i < ROWS; ++i) nxt[i] = 0;

        /* propagate along edges */
        for (int i = 0; i < ROWS; ++i) {
            const seq_t cur = dp[i];
            if (cur == 0) continue;
            int start = edge_start[i];
            int cnt   = edge_count[i];
            for (int k = 0; k < cnt; ++k) {
                int j = edge_list[start + k];
                nxt[j] += cur;
            }
        }

        /* swap dp and nxt */
        for (int i = 0; i < ROWS; ++i) dp[i] = nxt[i];
    }

    /* Sum over all ending rows */
    seq_t total = 0;
    for (int i = 0; i < ROWS; ++i) total += dp[i];
    return total;
}
