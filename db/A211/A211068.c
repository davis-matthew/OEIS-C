/*
 * A211068  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          positive odd determinant.
 * https://oeis.org/A211068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A211065(n)/2 Simplified: if n is even, a(n) = 3*n^4/16 if
 *            n is odd, a(n) = ((n+1)^2*(3n+1)*(n-1))/16 Offset: 1 (a(1) = 0)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 387, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once */
    seq_t nseq = (seq_t)n;

    /* Decide parity using the least significant bit */
    if ((n & 1ULL) == 0ULL)    /* n even */
    {
        seq_t n2  = nseq * nseq;   /* n^2   */
        seq_t n4  = n2 * n2;       /* n^4   */
        seq_t num = 3 * n4;        /* 3*n^4 */
        return num / 16;           /* divide by 16 */
    }
    else                       /* n odd */
    {
        seq_t nplus1  = nseq + 1;   /* n+1   */
        seq_t nminus1 = nseq - 1;   /* n-1   */
        seq_t term1   = nplus1 * nplus1;   /* (n+1)^2 */
        seq_t term2   = 3 * nseq + 1;       /* 3n+1 */
        seq_t num     = term1 * term2;      /* (n+1)^2*(3n+1) */
        num = num * nminus1;               /* * (n-1) */
        return num / 16;                   /* divide by 16 */
    }
}
