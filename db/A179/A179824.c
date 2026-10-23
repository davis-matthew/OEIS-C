/*
 * A179824  Chromatic polynomial of the star graph on 4 vertices (claw graph)
 *          and the path graph on 4 vertices.
 * https://oeis.org/A179824
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n-1)^3 (offset = 2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;       /* undefined for indices below the offset */
    seq_t nn = (seq_t)n;
    seq_t r = nn - 1;   /* (n-1) */
    return nn * r * r * r;  /* n * (n-1)^3 */
}
