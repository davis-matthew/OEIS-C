/*
 * A019433  Continued fraction for tan(1/10).
 * https://oeis.org/A019433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 0 a(1) = 9 a(n) = 1 for n >= 2 and even a(n) = 10*n - 2
 *            for n >= 3 and odd (derived from a(n) = -1/2 + (3*(-1)^n)/2 +
 *            5*n - 5*(-1)^n*n for n>1)
 * Range:     u8 n <= 26, u16 n <= 6554, u32 n <= 9354, u64 n <= 9354,
 *            u128 n <= 9354, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)9;
    if ((n & 1ULL) == 0)  /* even n >= 2 */
        return (seq_t)1;
    /* odd n >= 3 */
    return (seq_t)((seq_t)10 * (seq_t)n - (seq_t)2);
}
