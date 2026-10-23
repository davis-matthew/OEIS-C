/*
 * A019431  Continued fraction for tan(1/8).
 * https://oeis.org/A019431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 0, a(1) = 7, a(n) = (-1 + 3*(-1)^n - 8*(-1 + (-1)^n)*n) /
 *            2 for n > 1
 * Range:     u8 n <= 32, u16 n <= 8192, u32 n <= 9992, u64 n <= 9992,
 *            u128 n <= 9992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)7;

    /* Use signed intermediate to avoid overflow */
    long long sign1 = ((n & 1) ? -1 : 1);          /* (-1)^n */
    long long sign2 = -1 + sign1;                  /* -1 + (-1)^n: 0 or -2 */
    long long numer = -1 + 3 * sign1 - 8 * sign2 * (long long)n;

    /* numer is always even and positive for n >= 2 */
    return (seq_t)(numer / 2);
}
