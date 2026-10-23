/*
 * A008530  Coordination sequence for 4-dimensional primitive di-isohexagonal
 *          orthogonal lattice.
 * https://oeis.org/A008530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 6 * n * (n^2 + 1) for n>0
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from 3*a(n) = (2n+1)^3 + (2n-1)^3 + (n+1)^3 + (n-1)^3 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nseq = (seq_t)n;
    seq_t sq   = nseq * nseq;
    seq_t res  = 6 * nseq * (sq + (seq_t)1);
    return res;
}
