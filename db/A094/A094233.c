/*
 * A094233  Number of closed walks of length n at a vertex of the cyclic graph
 *          on 9 nodes C_9.
 * https://oeis.org/A094233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of closed walks of length n at a vertex of the cyclic
 *            graph on 9 vertices C₉. Computed by dynamic programming: at each
 *            step the number of ways to reach a vertex equals the sum of
 *            counts of its two neighbours
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const int M = 9;              /* number of vertices in C₉               */
    seq_t cur[M], nxt[M];
    for (int i = 0; i < M; ++i) cur[i] = (seq_t)0;   /* zero‑init          */
    cur[0] = (seq_t)1;           /* start at vertex 0                       */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < M; ++i) {
            int left  = i - 1; if (left < 0)      left += M;
            int right = i + 1; if (right == M)    right -= M;
            nxt[i] = cur[left] + cur[right];
        }
        for (int i = 0; i < M; ++i) cur[i] = nxt[i];
    }

    return cur[0];  /* number of walks returning to the start vertex */
}
