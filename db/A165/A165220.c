/*
 * A165220  Numbers n such that 8*n+1 is a cube.
 * https://oeis.org/A165220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(24*n+3+64*n^2)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 406, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;          /* n^2 */
    seq_t term = (seq_t)24 * nseq;     /* 24*n */
    term = term + (seq_t)3;            /* +3 */
    term = term + (seq_t)64 * n2;      /* +64*n^2 */
    return nseq * term;                /* n * term */
}
