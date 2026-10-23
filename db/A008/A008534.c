/*
 * A008534  Coordination sequence for {A_6}* lattice.
 * https://oeis.org/A008534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7/6)*n*(n^2+2)*(n^2+3)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 80, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t term = ((7 * nn) * (n2 + 2) * (n2 + 3)) / 6;
    return term;
}
