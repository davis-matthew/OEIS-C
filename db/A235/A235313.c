/*
 * A235313  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 7
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A235313
 *  number of (n+1)×3 arrays over {0,…,6} in which every 2×2 subblock
 *  has the property that |(top-left + bottom-right) – (top-right + bottom-left)|
 *  equals 7.
 *
 *  Result is computed by dynamic programming on a graph with 7³=343 vertices.
 *  A vertex corresponds to a row (x0,x1,x2).  There is an edge from a vertex
 *  to a vertex (y0,y1,y2) iff the two overlapping 2×2 blocks defined by
 *  the pair of rows satisfy the above absolute‑difference condition.
 *  The number of arrays of n+1 rows equals the number of walks of length n
 *  in this graph, starting from any vertex.
 *
 *  The adjacency lists are built once, in O(7⁶)=O(117 649) time.
 *  Each DP step then touches at most 343×maxDegree edges; with the
 *  maximum degree 343 this is at most 1 176 649 elementary operations per step,
 *  well within the limits for n≈10 000.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define STATE_COUNT 343          /* 7³ */
#define MAX_NEIGH    STATE_COUNT /* at most 343 neighbours per state */

static uint16_t to_list[STATE_COUNT][MAX_NEIGH];
static uint16_t neigh_cnt[STATE_COUNT];
static bool     adj_initialized = false;

static void init_adj(void)
{
    if (adj_initialized)
        return;

    /* precompute all admissible transitions */
    for (uint16_t cur = 0; cur < STATE_COUNT; ++cur) {
        /* decode current row components */
        uint16_t x0 = cur / 49;
        uint16_t x1 = (cur / 7) % 7;
        uint16_t x2 = cur % 7;

        uint16_t cnt = 0;
        for (uint16_t nxt = 0; nxt < STATE_COUNT; ++nxt) {
            /* decode next row components */
            uint16_t y0 = nxt / 49;
            uint16_t y1 = (nxt / 7) % 7;
            uint16_t y2 = nxt % 7;

            int d0 = (int)(x0 + y1) - (int)(x1 + y0);
            if (d0 < 0)
                d0 = -d0;
            if (d0 != 7)
                continue;               /* first block fails */

            int d1 = (int)(x1 + y2) - (int)(x2 + y1);
            if (d1 < 0)
                d1 = -d1;
            if (d1 != 7)
                continue;               /* second block fails */

            /* both conditions satisfied */
            to_list[cur][cnt++] = nxt;
        }
        neigh_cnt[cur] = cnt;
    }

    adj_initialized = true;
}

/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined offset; safe to return 0 */

    init_adj();

    static seq_t cur[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];

    /* any first row is allowed */
    for (uint16_t i = 0; i < STATE_COUNT; ++i)
        cur[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        /* zero next array */
        for (uint16_t i = 0; i < STATE_COUNT; ++i)
            nxt[i] = 0;

        /* propagate transitions */
        for (uint16_t i = 0; i < STATE_COUNT; ++i) {
            seq_t val = cur[i];
            if (val == 0)
                continue;
            uint16_t cnt = neigh_cnt[i];
            for (uint16_t k = 0; k < cnt; ++k) {
                uint16_t j = to_list[i][k];
                nxt[j] += val;
            }
        }

        /* copy back for next iteration */
        for (uint16_t i = 0; i < STATE_COUNT; ++i)
            cur[i] = nxt[i];
    }

    /* sum over all ending states gives the total number of arrays */
    seq_t total = 0;
    for (uint16_t i = 0; i < STATE_COUNT; ++i)
        total += cur[i];

    return total;
}
