/*
 * A111658  Pronic residues. These are analogous to quadratic residues, but
 *          use the set of pronics 0 2 6 12 20 30... instead of the quadratics
 *          (squares) 0 1 4 9 16 25... The example shown above is mod 13.
 * https://oeis.org/A111658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1) mod 13
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    return (k * (k + 1)) % (seq_t)13;
}
