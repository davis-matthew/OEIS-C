/*
 * A008528  Coordination sequence for 4-dimensional RR-centered
 *          di-isohexagonal orthogonal lattice.
 * https://oeis.org/A008528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(11*n^2 + 7), with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 730, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    /* compute 11*n^2 + 7 safely in seq_t */
    seq_t term = ((seq_t)11 * nn * nn) + (seq_t)7;
    return nn * term;
}
