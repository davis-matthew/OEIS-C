/*
 * A212975  Number of (w,x,y) with all terms in {0,...,n} and even range.
 * https://oeis.org/A212975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n*(n+2)+3*(-1)^n+1)/4
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for all arithmetic */
    seq_t N = (seq_t)n;

    /* compute 2*n*(n+2) */
    seq_t t1 = 2 * N * (N + 2);

    /* inner = 2*n*(n+2) + 1 + 3*(-1)^n
       For n even: inner = t1 + 4
       For n odd : inner = t1 - 2   */
    seq_t inner;
    if ((n & 1) == 0) {
        inner = t1 + 4;
    } else {
        inner = t1 - 2;
    }

    /* (n+1)*inner / 4 */
    seq_t result = (N + 1) * inner;
    return result / 4;
}
