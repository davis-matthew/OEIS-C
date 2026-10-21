/*
 * A085001  a(n) = (3*n+1)*(3*n+4).
 * https://oeis.org/A085001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*(3*n+4)
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term1 = nseq * 3 + 1;
    seq_t term2 = nseq * 3 + 4;
    return term1 * term2;
}
