/*
 * A061222  a(n) = n^2 + (n + 1)^3 + (n + 2)^4.
 * https://oeis.org/A061222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + (n + 1)^3 + (n + 2)^4
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* promote idx_t to seq_t for all arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t nplus1 = nseq + (seq_t)1;
    seq_t nplus2 = nseq + (seq_t)2;

    /* compute n^2 */
    seq_t term1 = nseq * nseq;

    /* compute (n+1)^3 */
    seq_t term2 = nplus1 * nplus1 * nplus1;

    /* compute (n+2)^4 as ((n+2)^2)^2 */
    seq_t nplus2_sq = nplus2 * nplus2;
    seq_t term3 = nplus2_sq * nplus2_sq;

    return term1 + term2 + term3;
}
