/*
 * A173963  Number of nonoverlapping placements of one 1 X 1 square and one 2
 *          X 2 square on an n X n board.
 * https://oeis.org/A173963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 4) * (n-1)^2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t t1   = nseq * nseq - (seq_t)4;      /* n^2 - 4   */
    seq_t t2   = (nseq - (seq_t)1);           /* n-1       */
    t2         = t2 * t2;                    /* (n-1)^2   */
    return t1 * t2;                          /* product   */
}
