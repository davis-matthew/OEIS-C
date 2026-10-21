/*
 * A223265  Rolling cube footprints: number of n X 4 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves across a
 *          corresponding cube edge.
 * https://oeis.org/A223265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 112*a(n-1) - 1024*a(n-2) Initial values: a(1) = 64, a(2)
 *            = 6144
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)64;
    if (n == 2)
        return (seq_t)6144;

    seq_t a1 = (seq_t)64;      /* a(n-2) */
    seq_t a2 = (seq_t)6144;    /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)112 * a2 - (seq_t)1024 * a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
