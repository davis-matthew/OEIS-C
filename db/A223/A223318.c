/*
 * A223318  Rolling icosahedron footprints: number of n X 5 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves along an icosahedral edge.
 * https://oeis.org/A223318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 479*a(n-1) - 15210*a(n-2), with a(1) = 625, a(2) = 274625
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (unused but safe) */
    if (n < 1) return (seq_t)0;

    /* Handle the first two terms directly */
    if (n == 1) return (seq_t)625;
    if (n == 2) return (seq_t)274625;

    /* Iterate from 3 up to n, keeping only the last two values */
    seq_t prev2 = (seq_t)625;        /* a(n-2) */
    seq_t prev1 = (seq_t)274625;     /* a(n-1) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)479 * prev1 - (seq_t)15210 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
