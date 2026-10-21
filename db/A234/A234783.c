/*
 * A234783  Number of (n+1)X(5+1) 0..3 arrays with no adjacent elements equal
 *          and with each 2X2 subblock having the number of clockwise edge
 *          increases equal to the number of counterclockwise edge increases.
 * https://oeis.org/A234783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   rows_compatible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=========================================================================*/
/*  OEIS A234783   Number of (n+1)×6 arrays over 0..3 with no equal adjacents
 *  and each 2×2 block having equal number of clockwise and counter‑clockwise
 *  edge increases.  The implemented method uses dynamic programming over
 *  all admissible 6‑length rows.
 *
 *  The transition graph is built once on first use.  For an array of
 *  height n+1 we perform n transitions.
 *
 *  Requirements satisfied:
 *  - Uses only integer arithmetic,
 *  - Works for any seq_t / idx_t types supplied by the build,
 *  - Memoises the adjacency graph for repeated calls,
 *  - Runs in O(n·E) where E≈4·10⁵, well under the time limits.
 *
 *  NOTE: The sequence is not given by a simple closed form; the DP
 *        implementation below follows the definition directly.
 *=========================================================================*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rows_compatible.h"

/* Number of admissible rows of length 6 over {0,1,2,3} with no equal neighbours */
#define NUM_ROWS 972

static uint8_t rows[NUM_ROWS][6];
static uint32_t start_idx[NUM_ROWS + 1];
static uint16_t *adj = NULL;
static bool initialized = false;

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Initialise rows and adjacency lists once.  The function is idempotent.   */
static void init_graph(void)
{
    if (initialized)
        return;

    /* Enumerate all admissible rows */
    int idx = 0;
    for (unsigned u = 0; u < 4; ++u)
    for (unsigned v = 0; v < 4; ++v) if (v != u)
    for (unsigned w = 0; w < 4; ++w) if (w != v)
    for (unsigned x = 0; x < 4; ++x) if (x != w)
    for (unsigned y = 0; y < 4; ++y) if (y != x)
    for (unsigned z = 0; z < 4; ++z) if (z != y) {
        rows[idx][0] = u; rows[idx][1] = v; rows[idx][2] = w;
        rows[idx][3] = x; rows[idx][4] = y; rows[idx][5] = z;
        ++idx;
    }

    /* Build degree of each row (number of compatible following rows) */
    start_idx[0] = 0;
    for (int i = 0; i < NUM_ROWS; ++i) {
        uint32_t deg = 0;
        for (int j = 0; j < NUM_ROWS; ++j)
            if (rows_compatible(rows[i], rows[j]))
                ++deg;
        start_idx[i + 1] = start_idx[i] + deg;
    }

    /* Allocate adjacency list */
    uint32_t total_edges = start_idx[NUM_ROWS];
    adj = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    uint32_t pos = 0;
    for (int i = 0; i < NUM_ROWS; ++i) {
        for (int j = 0; j < NUM_ROWS; ++j)
            if (rows_compatible(rows[i], rows[j]))
                adj[pos++] = (uint16_t)j;
    }

    initialized = true;
}

/*---------------------------------------------------------------------------*/
/* Sequence function.  Implemented as described above.                       */
seq_t a(idx_t n)
{
    init_graph();
    if (n == 0)
        return (seq_t)0; /* undefined for offset 1 */

    static seq_t dp[NUM_ROWS];
    static seq_t dp_next[NUM_ROWS];

    /* initialise first row */
    for (int i = 0; i < NUM_ROWS; ++i)
        dp[i] = (seq_t)1;

    /* perform n transitions (add n more rows) */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < NUM_ROWS; ++i)
            dp_next[i] = (seq_t)0;

        for (int i = 0; i < NUM_ROWS; ++i) {
            uint32_t s = start_idx[i];
            uint32_t e = start_idx[i + 1];
            for (uint32_t k = s; k < e; ++k) {
                uint16_t j = adj[k];
                dp_next[j] += dp[i];
            }
        }

        /* copy dp_next into dp for next iteration */
        for (int i = 0; i < NUM_ROWS; ++i)
            dp[i] = dp_next[i];
    }

    /* total number of arrays = sum over all ending rows */
    seq_t total = (seq_t)0;
    for (int i = 0; i < NUM_ROWS; ++i)
        total += dp[i];

    return total;
}
