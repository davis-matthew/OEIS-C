/*
 * A108507  Number of paths of length n between two arbitrary, distinct
 *          vertices in K5, the complete graph on 5 vertices.
 * https://oeis.org/A108507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of edge‑simple walks of length n between two distinct
   vertices in the complete graph K5.  The number is zero for n>9.
   For n≤9 the value is computed by dynamic programming over the
   10 edges of K5.  The symmetry of K5 allows fixing start=0,
   target=1. */
seq_t a(idx_t n)
{
    /* Edge indices (i<j).  -1 on the diagonal. */
    static const int EDGE[5][5] = {
        { -1, 0, 1, 2, 3 },
        {  0, -1, 4, 5, 6 },
        {  1, 4, -1, 7, 8 },
        {  2, 5, 7, -1, 9 },
        {  3, 6, 8, 9, -1 }
    };
    const int START = 0;
    const int TARGET = 1;
    const int MASKS = 1 << 10;          /* 2^10 = 1024 */

    /* Paths longer than 9 are impossible with distinct edges. */
    if (n < 1 || n > 9) {
        return (seq_t)0;
    }

    /* DP arrays: dp[cur][vertex][mask] */
    seq_t dp[2][5][MASKS];
    int cur = 0, nxt = 1;
    /* initialise all entries to 0 */
    for (int v = 0; v < 5; ++v)
        for (int m = 0; m < MASKS; ++m)
            dp[cur][v][m] = (seq_t)0;

    /* start at vertex 0 with no edges used */
    dp[cur][START][0] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        /* zero next layer */
        for (int v = 0; v < 5; ++v)
            for (int m = 0; m < MASKS; ++m)
                dp[nxt][v][m] = (seq_t)0;

        for (int v = 0; v < 5; ++v) {
            for (int m = 0; m < MASKS; ++m) {
                seq_t val = dp[cur][v][m];
                if ((seq_t)0 == val) continue;
                for (int w = 0; w < 5; ++w) {
                    if (w == v) continue;
                    int e = EDGE[v][w];
                    if (e < 0) continue;          /* just in case */
                    if (m & (1 << e)) continue;   /* edge already used */
                    int newm = m | (1 << e);
                    dp[nxt][w][newm] += val;
                }
            }
        }
        /* swap layers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all masks that end at TARGET */
    seq_t result = (seq_t)0;
    for (int m = 0; m < MASKS; ++m) {
        result += dp[cur][TARGET][m];
    }
    return result;
}
