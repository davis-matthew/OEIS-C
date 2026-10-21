/*
 * A062077  a(n) = (2n)^n * n^(2n).
 * https://oeis.org/A062077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (2*n)^n * n^(2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (2n)^n */
    seq_t two_n = (seq_t)(2 * n);
    seq_t pow2n = 1;
    for (idx_t i = 0; i < n; ++i) pow2n *= two_n;

    /* n^(2n) */
    seq_t nseq = (seq_t)n;
    seq_t pown = 1;
    for (idx_t i = 0; i < 2 * n; ++i) pown *= nseq;

    return pow2n * pown;
}
