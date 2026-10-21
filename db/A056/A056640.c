/*
 * A056640  At stage 1, start with a unit square. At each successive stage add
 *          4*(n-1) new squares around outside with edge-to-edge contacts.
 *          Sequence gives number of squares (regardless of size) at n-th
 *          stage.
 * https://oeis.org/A056640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3-2*n+3-3*(-1)^n)/12
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t val = 8 * N3;
    val -= 2 * N;
    if (n & 1) {
        val += 6;
    }
    return val / 12;
}
