/*
 * A024862  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n-k+1), where k =
 *          floor(n/2), s = natural numbers, t = odd natural numbers.
 * https://oeis.org/A024862
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (4*n^3 + 9*n^2 + 2*n)/24 for n even * a(n) = (4*n^3 +
 *            3*n^2 - 4*n - 3)/24 for n odd * Where n is the index (a(2) is
 *            the first term, n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; the behaviour is undefined
     * for n < 2 according to the OEIS specification. */
    if (n < 2)
        return 0;

    seq_t nseq = (seq_t)n;                     /* convert index to seq_t   */
    seq_t n2   = nseq * nseq;                  /* n^2                     */
    seq_t n3   = n2 * nseq;                    /* n^3                     */

    seq_t numerator;
    if ((n & 1) == 0) {                       /* n even   */
        numerator = 4 * n3;                   /* 4*n^3   */
        numerator += 9 * n2;                  /* +9*n^2  */
        numerator += 2 * nseq;                /* +2*n    */
    } else {                                  /* n odd    */
        numerator = 4 * n3;                   /* 4*n^3   */
        numerator += 3 * n2;                  /* +3*n^2  */
        seq_t sub = 4 * nseq + 3;             /* 4*n + 3 */
        numerator -= sub;                     /* -4*n - 3*/
    }

    /* Division by 24 is exact for all n. */
    return numerator / 24;
}
