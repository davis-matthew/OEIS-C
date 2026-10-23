/*
 * A187297  Number of 2-step one space leftwards or up, two space rightwards
 *          or down asymmetric rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n == 1; otherwise a(n) = 4*n*n - 6*n
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    /* use 64-bit intermediate to avoid overflow for n up to at least 10000 */
    idx_t nn = n;
    idx_t val = 4 * nn * nn - 6 * nn;
    return (seq_t)val;
}
