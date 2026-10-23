/*
 * A188386  a(n) = numerator(H(n+2)-H(n-1)), where H(n) = Sum_{k=1..n} 1/k is
 *          the n-th harmonic number.
 * https://oeis.org/A188386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = numerator(H(n+2)-H(n-1)) = * (3-(-1)^n)*(3*n^2+6*n+2)/4 *
 *            For odd n the factor is 4, so a(n)=3*n^2+6*n+2; * for even n the
 *            factor is 2, so a(n)=(3*n^2+6*n+2)/2
 * Range:     u8 n <= 8, u16 n <= 146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n < 1 is not defined; return 0 for safety. */
    if (n == 0) return 0;
    seq_t N = n;               /* cast to seq_t for all arithmetic */
    seq_t tmp = 3 * N * N;     /* 3*n^2  */
    tmp += 6 * N;              /* +6*n   */
    tmp += 2;                  /* +2     */
    if ((N & 1) == 0) {        /* n even */
        return tmp / 2;        /* divide by 2 */
    } else {                   /* n odd  */
        return tmp;            /* no division */
    }
}
