/*
 * A190577  a(n) = n*(n+2)*(n+4)*(n+6).
 * https://oeis.org/A190577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)*(n+4)*(n+6)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 253, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t res = nseq * (nseq + 2);
    res *= (nseq + 4);
    res *= (nseq + 6);
    return res;
}
