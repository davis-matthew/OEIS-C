/*
 * A123482  Coefficients of the series giving the best rational approximations
 *          to sqrt(11).
 * https://oeis.org/A123482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 399*a(n+2) - 399*a(n+1) + a(n) with a(1)=60,
 *            a(2)=23940, a(3)=9528120
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)60;
    if (n == 2) return (seq_t)23940;
    if (n == 3) return (seq_t)9528120;

    seq_t a0 = (seq_t)60;            /* a(n-3) */
    seq_t a1 = (seq_t)23940;          /* a(n-2) */
    seq_t a2 = (seq_t)9528120;        /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)399 * a2 - (seq_t)399 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
