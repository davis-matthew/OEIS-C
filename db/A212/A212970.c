/*
 * A212970  Number of (w,x,y) with all terms in {0,...,n} and w != x and x <
 *          range(w,x,y).
 * https://oeis.org/A212970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^3 + 24*n^2 + 8*n + (6*n)*(n mod 2))/24
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term = (seq_t)10 * nseq * nseq * nseq;   /* 10*n^3 */
    term += (seq_t)24 * nseq * nseq;               /* +24*n^2 */
    term += (seq_t)8 * nseq;                      /* +8*n   */
    if (n & 1)                                    /* if n is odd add 6*n */
        term += (seq_t)6 * nseq;
    return term / (seq_t)24;
}
