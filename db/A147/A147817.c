/*
 * A147817  Number of consistent sets of 4 irreflexive binary order
 *          relationships over n objects.
 * https://oeis.org/A147817
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,4) * (n^4 + 2*n^3 - 5*n^2 - 22*n - 30)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;               /* the sequence is defined for n ≥ 4 */

    /* cast n to seq_t for all arithmetic */
    seq_t nseq = (seq_t)n;

    /* compute the polynomial part: n^4 + 2*n^3 - 5*n^2 - 22*n - 30 */
    seq_t n2 = nseq * nseq;          /* n^2  */
    seq_t n3 = n2  * nseq;          /* n^3  */
    seq_t n4 = n3  * nseq;          /* n^4  */
    seq_t poly = n4
                + (seq_t)2 * n3
                - (seq_t)5 * n2
                - (seq_t)22 * nseq
                - (seq_t)30;      /* polynomial value */

    /* compute binomial(n,4) via multiplicative formula
       binom = n*(n-1)*(n-2)*(n-3)/24  with intermediate division to keep
       the intermediates small.  The operations are in seq_t, so overflows
       are modulo the width of seq_t, which is acceptable for the required
       precision. */
    seq_t comb = 1;
    comb = comb * (seq_t)(n - 3) / 1;   /* multiply by (n-3)   */
    comb = comb * (seq_t)(n - 2) / 2;   /* multiply by (n-2)/2 */
    comb = comb * (seq_t)(n - 1) / 3;   /* multiply by (n-1)/3 */
    comb = comb * (seq_t)(n)     / 4;   /* multiply by n/4     */

    return comb * poly;
}
