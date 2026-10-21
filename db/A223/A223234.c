/*
 * A223234  Rolling icosahedron footprints: number of 2 X n 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves along an icosahedral edge.
 * https://oeis.org/A223234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 30*a(n-2) (n > 3), with initial values a(1) =
 *            12, a(2) = 65, a(3) = 785
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)65;
    if (n == 3) return (seq_t)785;

    seq_t prev2 = (seq_t)65;   /* a(2)     */
    seq_t prev1 = (seq_t)785;  /* a(3)     */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)7 * prev1 + (seq_t)30 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
