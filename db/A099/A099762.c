/*
 * A099762  a(n) = n^2 * (n+1)^3.
 * https://oeis.org/A099762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 * (n+1)^3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = n;
    seq_t m = nseq + 1;
    seq_t result = nseq * nseq;
    result *= m;
    result *= m;
    result *= m;
    return result;
}
