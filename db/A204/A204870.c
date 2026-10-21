/*
 * A204870  Number of (n+1) X 3 0..3 arrays with the permanents of all 2 X 2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 whose * every
 *            2×2 sub‑block has equal non‑zero permanents. The value is *
 *            computed by counting walks of length n in a directed graph whose
 *            * vertices are the 64 triples (a,b,c), edges represent a step to
 *            the * next row and are labelled by the comm...
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The build supplies these types */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* Edge: from → to with permanent label index pidx */
struct Edge { uint8_t from; uint8_t to; uint8_t pidx; } edges[4096];
static int edge_cnt = 0;
static int p_map[19] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
static int p_count  = 0;
static bool init_done = false;

/* One‑time build of the graph */
static void init_graph(void)
{
    if (init_done) return;
    for (int i = 0; i < 19; ++i) p_map[i] = -1;
    edge_cnt = 0; p_count = 0;

    /* enumerate all 64 triples (a,b,c) with entries 0..3 */
    for (int a = 0; a < 4; ++a)
    for (int b = 0; b < 4; ++b)
    for (int c = 0; c < 4; ++c) {
        int s = a + (b << 2) + (c << 4);   /* encode one state */

        for (int a2 = 0; a2 < 4; ++a2)
        for (int b2 = 0; b2 < 4; ++b2)
        for (int c2 = 0; c2 < 4; ++c2) {
            int t = a2 + (b2 << 2) + (c2 << 4);  /* target state */

            int p1 = a * b2 + b * a2;
            int p2 = b * c2 + c * b2;
            if (p1 == p2 && p1 > 0) {
                int val = p1;
                int idx = p_map[val];
                if (idx == -1) {
                    idx = p_count;
                    p_map[val] = idx;
                    ++p_count;
                }
                edges[edge_cnt].from = (uint8_t)s;
                edges[edge_cnt].to   = (uint8_t)t;
                edges[edge_cnt].pidx = (uint8_t)idx;
                ++edge_cnt;
            }
        }
    }
    init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_graph();

    /* DP arrays: 20 possible P values (enough for 1..18) × 64 states */
    static seq_t dp_cur[20][64] = {0};
    static seq_t dp_next[20][64] = {0};

    /* start with paths of length 1 (one edge) */
    for (int i = 0; i < p_count; ++i)
        for (int s = 0; s < 64; ++s)
            dp_cur[i][s] = 0;

    for (int e = 0; e < edge_cnt; ++e) {
        int pidx = edges[e].pidx;
        int to   = edges[e].to;
        dp_cur[pidx][to] = dp_cur[pidx][to] + 1;
    }

    /* If only one edge is needed */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < p_count; ++i)
            for (int s = 0; s < 64; ++s)
                total = total + dp_cur[i][s];
        return total;
    }

    /* Iterate additional n-1 edges */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < p_count; ++i)
            for (int s = 0; s < 64; ++s)
                dp_next[i][s] = 0;

        /* transition */
        for (int e = 0; e < edge_cnt; ++e) {
            int pidx = edges[e].pidx;
            int from = edges[e].from;
            int to   = edges[e].to;
            dp_next[pidx][to] = dp_next[pidx][to] + dp_cur[pidx][from];
        }

        /* copy back to dp_cur */
        for (int i = 0; i < p_count; ++i)
            for (int s = 0; s < 64; ++s)
                dp_cur[i][s] = dp_next[i][s];
    }

    /* sum all counts */
    seq_t total = 0;
    for (int i = 0; i < p_count; ++i)
        for (int s = 0; s < 64; ++s)
            total = total + dp_cur[i][s];
    return total;
}
