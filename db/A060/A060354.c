/*
 * A060354  The n-th n-gonal number: a(n) = n*(n^2 - 3*n + 4)/2.
 * https://oeis.org/A060354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n*(n^2 - 3*n + 4)/2
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;            /* n^2          */
    seq_t tmp = n2 - 3 * nseq + 4;     /* n^2 - 3n + 4 */
    return (nseq * tmp) / 2;           /* n*(n^2-3n+4)/2 */
}
