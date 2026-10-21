/*
 * A212897  Number of (w,x,y,z) with all terms in {0,...,n} and (least
 *          gapsize)>1.
 * https://oeis.org/A212897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4-5*n^3+12*n^2-16*n+10, with a(0)=a(1)=0
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;          /* n^2 */
    seq_t n3 = n2 * nseq;            /* n^3 */
    seq_t n4 = n3 * nseq;            /* n^4 */

    seq_t val = n4;                  /* start with n^4 */
    val -= (seq_t)5 * n3;            /* subtract 5*n^3 */
    val += (seq_t)12 * n2;           /* add 12*n^2 */
    val -= (seq_t)16 * nseq;         /* subtract 16*n */
    val += (seq_t)10;                /* add constant term */

    return val;
}
