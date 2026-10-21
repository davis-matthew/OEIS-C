/*
 * A071303  1/2 times the number of n X n 0..3 matrices M with MM' mod 4 = I,
 *          where M' is the transpose of M and I is the n X n identity matrix.
 * https://oeis.org/A071303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n! * 2^(n*(n+1)/2 - 1 + max(0,(n-4)*(n-5)/2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* exponent of the power of 2 */
    idx_t e = n * (n + 1) / 2 - 1;     /* first part: binomial(n+1,2)-1 */
    if (n > 5)                         /* add C(n-4,2) = (n-4)(n-5)/2 when n≥6 */
        e += (n - 4) * (n - 5) / 2;

    return fact << e;                   /* fact * 2^e */
}
