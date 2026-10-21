/*
 * A080988  Define b by b(1) = 1 and for n>1, b(n) = b(n-1)+1/(3+1/b(n-1));
 *          sequence gives numerator of b(n).
 * https://oeis.org/A080988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(1)=1. Let m=3. Define fractions b(k)=n(k)/d(k) by * n(1)=1,
 *            d(1)=1, and for k≥1 * n(k+1) = n(k) * (m*n(k) + 2*d(k)) * d(k+1)
 *            = d(k) * (m*n(k) + d(k)) * The sequence A080988 gives the
 *            numerators n(k). This function * computes a(n) iteratively. It
 *            works for any seq_t that supports * addition, mul...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* n(1) = 1 */

    seq_t num = (seq_t)1;         /* current numerator n(k) */
    seq_t den = (seq_t)1;         /* current denominator d(k) */
    const seq_t m = (seq_t)3;     /* constant m = 3 */

    for (idx_t i = 1; i < n; ++i) {
        /* t1 = m*num + 2*den, t2 = m*num + den */
        seq_t t1 = m * num + (seq_t)2 * den;
        seq_t t2 = m * num + den;

        /* new numerator and denominator */
        seq_t new_num = num * t1;
        seq_t new_den = den * t2;

        num = new_num;
        den = new_den;
    }
    return num;   /* numerator of b(n) */
}
