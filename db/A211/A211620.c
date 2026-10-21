/*
 * A211620  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and -1<=2w+x+y<=1.
 * https://oeis.org/A211620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n = 0; otherwise a(n) = 6*n^2 - 6*n + 4 for even n,
 *            6*n^2 - 6*n + 2 for odd n
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t nseq = (seq_t)n;
    seq_t sq   = nseq * nseq;            /* n^2 */
    seq_t res  = sq * (seq_t)6 - nseq * (seq_t)6; /* 6*n^2 - 6*n */
    if ((n & 1ULL) == 0) res += (seq_t)4; /* even n */
    else                res += (seq_t)2; /* odd n  */
    return res;
}
