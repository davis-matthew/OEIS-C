/*
 * A223278  Rolling icosahedron face footprints: number of n X 4 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves across an icosahedral edge.
 * https://oeis.org/A223278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 36*a(n-2); a(1)=27, a(2)=351
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)351;

    seq_t prevprev = (seq_t)27;   /* a(1) */
    seq_t prev     = (seq_t)351;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)17 * prev - (seq_t)36 * prevprev;
        prevprev = prev;
        prev = cur;
    }

    return prev;
}
