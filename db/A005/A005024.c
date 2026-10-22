/*
 * A005024  Number of walks of length 2n+8 in the path graph P_9 from one end
 *          to the other.
 * https://oeis.org/A005024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = number of walks of length 2n+8 from vertex 1 to vertex 9
 *            in the path graph P_9, computed by dynamic programming over walk
 *            length: w_{t+1}(v) = w_t(v-1) + w_t(v+1), w_0 = e_1;
 *            equivalently a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur[11], nxt[11];
    int v;
    idx_t t, len;
    for (v = 0; v < 11; v++) { cur[v] = 0; nxt[v] = 0; }
    cur[1] = 1;
    len = 2 * n + 8;
    for (t = 0; t < len; t++) {
        for (v = 1; v <= 9; v++) nxt[v] = cur[v - 1] + cur[v + 1];
        for (v = 1; v <= 9; v++) cur[v] = nxt[v];
    }
    return cur[9];
}
