/*
 * A223460  6 X 6 X 6 triangular graph without horizontal edges coloring a
 *          rectangular array: number of n X 1 0..20 arrays where 0..20 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 3,6 3,7 4,7
 *          4,8 5,8 5,9 6,10 6,11 7,11 7,12 8,12 8,13 9,13 9,14 10,15 10,16
 *          11,16 11,17 12,17 12,18 13,18 13,19 14,1...
 * https://oeis.org/A223460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of walks of length n on the graph defined in the
 *            OEIS entry. This is computed by dynamic programming: a(1) = 21
 *            (one walk of length 1 starting at each vertex), a(n) = Σ_{v}
 *            cur_{n-1}(v), where cur_{k}(v) = Σ_{u∈N(v)} cur_{k-1}(u)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* adjacency list of the 21-node graph (max 4 neighbors per node) */
    static const int neighbors[21][4] = {
        {1,  2, -1, -1},   /* 0 */
        {0,  3,  4, -1},   /* 1 */
        {0,  4,  5, -1},   /* 2 */
        {1,  6,  7, -1},   /* 3 */
        {1,  2,  7,  8},   /* 4 */
        {2,  8,  9, -1},   /* 5 */
        {3, 10, 11, -1},   /* 6 */
        {3,  4, 11, 12},   /* 7 */
        {4,  5, 12, 13},   /* 8 */
        {5, 13, 14, -1},   /* 9 */
        {6, 15, 16, -1},   /*10 */
        {6,  7, 16, 17},   /*11 */
        {7,  8, 17, 18},   /*12 */
        {8,  9, 18, 19},   /*13 */
        {9, 19, 20, -1},   /*14 */
        {10, -1, -1, -1},  /*15 */
        {10, 11, -1, -1},  /*16 */
        {11, 12, -1, -1},  /*17 */
        {12, 13, -1, -1},  /*18 */
        {13, 14, -1, -1},  /*19 */
        {14, 20, -1, -1}   /*20 */
    };
    static const int deg[21] = {2,3,3,3,4,3,3,4,4,3,3,4,4,4,3,1,2,2,2,2,1};

    seq_t cur[21];
    seq_t nxt[21];
    /* initialize: one walk of length 1 ending at each vertex */
    for (int v = 0; v < 21; ++v) cur[v] = 1;

    /* propagate walks for lengths 2..n */
    for (idx_t step = 1; step < n; ++step) {
        for (int v = 0; v < 21; ++v) {
            seq_t sum = 0;
            for (int k = 0; k < deg[v]; ++k) {
                int u = neighbors[v][k];
                sum += cur[u];
            }
            nxt[v] = sum;
        }
        /* prepare for next step */
        for (int v = 0; v < 21; ++v) cur[v] = nxt[v];
    }

    /* total number of walks of length n */
    seq_t total = 0;
    for (int v = 0; v < 21; ++v) total += cur[v];
    return total;
}
