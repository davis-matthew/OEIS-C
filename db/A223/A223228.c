/*
 * A223228  Rolling icosahedron footprints: number of n X 3 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves along an icosahedral edge.
 * https://oeis.org/A223228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*a(n-1) - 90*a(n-2), a(1)=25, a(2)=785
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)25;
    if (n == (idx_t)2) return (seq_t)785;
    seq_t prev2 = (seq_t)25;       /* a(1) */
    seq_t prev1 = (seq_t)785;      /* a(2) */
    seq_t curr;
    for (idx_t i = (idx_t)3; i <= n; ++i) {
        curr = (seq_t)35 * prev1 - (seq_t)90 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
