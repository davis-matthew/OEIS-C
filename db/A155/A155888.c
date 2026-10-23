/*
 * A155888  a(n) = dimension of the space of n-boxes in the unshaded subfactor
 *          planar algebra of type E8.
 * https://oeis.org/A155888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+5) = 9*a(n+4) - 28*a(n+3) + 36*a(n+2) - 17*a(n+1) + 2*a(n) *
 *            with initial values a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14. *
 *            The function computes a(n) by counting length‑2n walks in the E8
 *            graph * starting and ending at the vertex farthest from the
 *            trivalent vertex
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* E8 graph adjacency:
     * vertices 0..7, vertex 7 is the farthest from the trivalent node 3.
     * The degree of each vertex is given by deg[].  neigh[][] lists
     * the adjacent vertices for each vertex. */
    static const int deg[8] = {1, 1, 2, 3, 2, 2, 2, 1};
    static const int neigh[8][3] = {
        {3, -1, -1},   /* 0 */
        {2, -1, -1},   /* 1 */
        {1, 3, -1},    /* 2 */
        {2, 4, 0},     /* 3 */
        {3, 5, -1},    /* 4 */
        {4, 6, -1},    /* 5 */
        {5, 7, -1},    /* 6 */
        {6, -1, -1}    /* 7 */
    };

    /* number of steps = 2n */
    idx_t steps;
    if (n > (idx_t)(~((idx_t)0) / 2))
        return (seq_t)0;       /* overflow guard; sequence undefined for such n */
    steps = 2 * n;

    /* current and next walk counts for each vertex */
    seq_t cur[8] = {0};
    seq_t next[8];

    /* start at vertex 7 */
    cur[7] = (seq_t)1;

    for (idx_t step = 0; step < steps; ++step) {
        /* reset next counts */
        next[0] = next[1] = next[2] = next[3] = next[4] = next[5] = next[6] = next[7] = (seq_t)0;

        /* propagate walks */
        for (int v = 0; v < 8; ++v) {
            seq_t val = cur[v];
            if (val == (seq_t)0)
                continue;
            for (int k = 0; k < deg[v]; ++k) {
                int nb = neigh[v][k];
                next[nb] += val;
            }
        }

        /* copy next → cur */
        cur[0] = next[0]; cur[1] = next[1]; cur[2] = next[2];
        cur[3] = next[3]; cur[4] = next[4]; cur[5] = next[5];
        cur[6] = next[6]; cur[7] = next[7];
    }

    return cur[7];
}
