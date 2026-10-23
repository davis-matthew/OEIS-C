/*
 * A184492  Number of (n+1)X5 0..2 arrays with every 2X2 subblock sum greater
 *          than 4.
 * https://oeis.org/A184492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184492
 *
 * a(n) = number of (n+1)×5 arrays with entries 0..2 such that every
 * 2×2 subblock sum > 4.  This is computed by counting walks of length n
 * in the directed graph whose vertices are the 3^5 = 243 possible rows
 * and whose edges connect rows that can follow each other while
 * preserving the 2×2‑block condition.
 *
 * The implementation builds the graph once, then uses a simple
 * dynamic‑programming step that runs in time O(n·E) with E ≈ 4000.
 * Memory is allocated statically so the code is valid both in C11
 * and C++17, even if seq_t is an arbitrary‑precision type.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Number of possible rows: 3^5 = 243. */
#define ROWS     243
#define COLS       5
#define BLOCKS     4          /* number of 2×2 blocks in a neighboring pair */

/* Row table: value of each column of each row. */
static uint8_t row_vals[ROWS][COLS];

/* Adjacency structure: adj_start[i] … adj_start[i+1] are the indices
 * in adj_list that can follow row i.  adj_list holds all destination
 * row indices.  The maximum possible number of edges is ROWS*ROWS,
 * so we allocate that many ints. */
static int  adj_start[ROWS + 1];
static int  adj_list[ROWS * ROWS];

/* Build the table of all rows and the adjacency graph.  The function
 * is executed only once. */
static void build_tables(void)
{
    int r, c;

    /* Encode all rows as base‑3 numbers. */
    for (r = 0; r < ROWS; ++r) {
        int x = r;
        for (c = COLS - 1; c >= 0; --c) {
            row_vals[r][c] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* Build adjacency lists. */
    int pos = 0;
    adj_start[0] = 0;
    for (r = 0; r < ROWS; ++r) {
        for (int s = 0; s < ROWS; ++s) {
            bool ok = true;
            for (int k = 0; k < BLOCKS; ++k) {
                int sum = row_vals[r][k]   + row_vals[r][k + 1]
                        + row_vals[s][k]   + row_vals[s][k + 1];
                if (sum <= 4) { ok = false; break; }
            }
            if (ok) adj_list[pos++] = s;
        }
        adj_start[r + 1] = pos;
    }
}

/* One‑time initialization guard. */
static bool tables_built = false;
static void init_tables(void)
{
    if (!tables_built) {
        build_tables();
        tables_built = true;
    }
}

/* ------------------------------------------------------------
 *  Main sequence function
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_tables();                         /* ensure tables are ready */

    /* Dynamic programming arrays: static to avoid malloc for
     * arbitrary‑precision types.  The arrays are overwritten on
     * every call, so static storage is safe. */
    static seq_t curr[ROWS];
    static seq_t next[ROWS];

    /* Initialise the first row counts: each row can start the array. */
    for (int i = 0; i < ROWS; ++i) curr[i] = (seq_t)1;

    /* Perform n transitions: for a(n) we need n steps to build
     * (n+1) rows. */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear next array. */
        for (int i = 0; i < ROWS; ++i) next[i] = (seq_t)0;

        /* Propagate counts along allowed edges. */
        for (int i = 0; i < ROWS; ++i) {
            seq_t cnt = curr[i];
            if (cnt == (seq_t)0) continue;
            int start = adj_start[i];
            int end   = adj_start[i + 1];
            for (int p = start; p < end; ++p) {
                int j = adj_list[p];
                next[j] += cnt;
            }
        }
        /* Swap curr and next for next iteration. */
        /* Using pointer swap would cause issues with static arrays,
         * so we copy the contents instead. */
        for (int i = 0; i < ROWS; ++i) curr[i] = next[i];
    }

    /* Sum all counts for final result. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < ROWS; ++i) result += curr[i];

    return result;
}
