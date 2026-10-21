/*
 * A223322  Rolling icosahedron footprints: number of 2 X n 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves along an icosahedral edge.
 * https://oeis.org/A223322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A223322: Rolling icosahedron footprints
   Formula: a(1) = 12, a(2) = 125, a(n) = 13 * a(n-1) for n > 2
   (equivalently a(n) = 125 * 13^(n-2) for n > 1) */
seq_t a(idx_t n)
{
    if (n == 1)
        return 12;
    if (n == 2)
        return 125;

    seq_t result = 125;          /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
        result *= 13;            /* multiply by factor 13 each step */

    return result;
}
