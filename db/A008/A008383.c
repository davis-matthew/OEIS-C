/*
 * A008383  Coordination sequence for A_4 lattice.
 * https://oeis.org/A008383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(7*n^2 + 5)/3 (with a(0) = 1)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nseq = (seq_t)n;
    seq_t t = 7 * nseq * nseq + (seq_t)5;
    t *= nseq;
    t *= (seq_t)5;
    return t / (seq_t)3;
}
