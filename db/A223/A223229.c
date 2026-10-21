/*
 * A223229  Rolling icosahedron footprints: number of n X 4 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves along an icosahedral edge.
 * https://oeis.org/A223229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 73*a(n-1) - 423*a(n-2) + 351*a(n-3) Initial values:
 *            a(1)=125, a(2)=7445, a(3)=492365
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)125;
    if (n == 2) return (seq_t)7445;
    if (n == 3) return (seq_t)492365;

    seq_t a1 = (seq_t)125;
    seq_t a2 = (seq_t)7445;
    seq_t a3 = (seq_t)492365;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = 73 * a3 - 423 * a2 + 351 * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
