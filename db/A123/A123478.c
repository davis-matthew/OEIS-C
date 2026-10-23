/*
 * A123478  Coefficients of series giving the best rational approximations to
 *          sqrt(7).
 * https://oeis.org/A123478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 255*a(n+2) - 255*a(n+1) + a(n); a(1)=48, a(2)=12240,
 *            a(3)=3108960
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset safely. */
    if (n == 0)
        return (seq_t)0;

    /* Return the first three terms directly. */
    if (n == 1)
        return (seq_t)48;
    if (n == 2)
        return (seq_t)12240;
    if (n == 3)
        return (seq_t)3108960;

    /* Initialize the recurrence state. */
    seq_t a1 = (seq_t)48;
    seq_t a2 = (seq_t)12240;
    seq_t a3 = (seq_t)3108960;

    /* Iterate the cubic recurrence to reach term n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a4 = (seq_t)255 * a3 - (seq_t)255 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a3;
}
