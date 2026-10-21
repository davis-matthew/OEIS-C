/*
 * A094855  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 4, s(2n+1) = 5.
 * https://oeis.org/A094855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of walks of length (2 n + 1) on the path graph P₈
 *            from vertex 4 to vertex 5 with steps of ±1 (vertices 1..8 are
 *            allowed)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* we perform a simple dynamic programming on the small graph */
    /* arrays indexed 1..8 (0 unused) */
    seq_t cur[9] = {0};
    seq_t nxt[9] = {0};

    /* start at vertex 4 */
    cur[4] = 1;

    /* number of steps = 2 n + 1 */
    idx_t steps = 2 * n + 1;   /* safe for all n of interest */

    for (idx_t step = 1; step <= steps; ++step) {
        /* reset next array */
        for (int v = 1; v <= 8; ++v) nxt[v] = 0;

        /* propagate current counts to neighbours */
        for (int v = 1; v <= 8; ++v) {
            /* skip zero entries */
            if (cur[v] == 0) continue;

            seq_t val = cur[v];
            if (v > 1)      nxt[v - 1] += val;   /* step left */
            if (v < 8)      nxt[v + 1] += val;   /* step right */
        }

        /* swap cur and nxt */
        for (int v = 1; v <= 8; ++v) cur[v] = nxt[v];
    }

    return cur[5];
}
