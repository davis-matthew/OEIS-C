/*
 * A025592  Number of n-move bishop paths on 8x8 board from given corner to
 *          any square.
 * https://oeis.org/A025592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n-move bishop paths starting from the corner on
 *            an 8×8 board. Computed by dynamic programming over board
 *            positions
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* adjacency precomputation */
    static int neighbors[64][13];
    static int neighbor_count[64];
    static int initialized = 0;
    if (!initialized) {
        const int dirs[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                int idx = r * 8 + c;
                int cnt = 0;
                for (int d = 0; d < 4; ++d) {
                    int dr = dirs[d][0];
                    int dc = dirs[d][1];
                    int nr = r + dr;
                    int nc = c + dc;
                    while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                        neighbors[idx][cnt++] = nr * 8 + nc;
                        nr += dr;
                        nc += dc;
                    }
                }
                neighbor_count[idx] = cnt;
            }
        }
        initialized = 1;
    }

    /* dynamic programming */
    seq_t f_prev[64];
    seq_t f_curr[64];
    for (int i = 0; i < 64; ++i) f_prev[i] = 0;
    f_prev[0] = 1; /* start at corner (0,0) */

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 64; ++i) f_curr[i] = 0;
        for (int q = 0; q < 64; ++q) {
            seq_t val = f_prev[q];
            if (val == 0) continue;
            int cnt = neighbor_count[q];
            for (int k = 0; k < cnt; ++k) {
                int dest = neighbors[q][k];
                f_curr[dest] += val;
            }
        }
        for (int i = 0; i < 64; ++i) f_prev[i] = f_curr[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < 64; ++i) ans += f_prev[i];
    return ans;
}
