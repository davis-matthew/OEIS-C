/*
 * A162993  The second left hand column of triangle A162990.
 * https://oeis.org/A162993
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)!/2)^2 (OEIS A162993), offset 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)          /* indices below offset are undefined */
        return (seq_t)0;

    if (n == 2)         /* first required value is 9 */
        return (seq_t)9;

    seq_t result = (seq_t)9;            /* a(2) */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t f = (seq_t)(k + 1);       /* k+1, cast to seq_t to avoid overflow in idx_t */
        seq_t mult = f * f;             /* (k+1)^2, all in seq_t arithmetic */
        result = result * mult;         /* recurrence a(k) = a(k-1)*(k+1)^2 */
    }
    return result;
}
