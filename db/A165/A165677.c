/*
 * A165677  Fifth right hand column of triangle A165674.
 * https://oeis.org/A165677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24 + 100*n + 105*n^2 + 40*n^3 + 5*n^4 (Offset: 1)
 * Range:     u8 none, u16 n <= 8, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index once to seq_t */
    seq_t nseq = (seq_t)n;

    /* pre‑compute powers of n */
    seq_t n2 = nseq * nseq;      /* n^2 */
    seq_t n3 = n2 * nseq;        /* n^3 */
    seq_t n4 = n3 * nseq;        /* n^4 */

    /* compute each term separately */
    seq_t term = (seq_t)24
                + (seq_t)100 * nseq
                + (seq_t)105 * n2
                + (seq_t)40 * n3
                + (seq_t)5 * n4;

    return term;
}
