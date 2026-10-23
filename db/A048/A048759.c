/*
 * A048759  Longest perimeter of a Pythagorean triangle with n as length of
 *          one of the three sides.
 * https://oeis.org/A048759
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1) for odd n; a(n) = n*(n+2)/2 for even n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // The sequence is defined for n >= 3; values for smaller n are undefined.
    if ((n & 1) == 0) {          /* n even: a(2*k) = 2*k*(k+1) */
        idx_t k = n >> 1;        /* k = n/2 */
        return (seq_t)k * (seq_t)(k + 1) * 2;
    } else {                     /* n odd:  a(2*k+1) = n*(n+1) */
        seq_t sn = (seq_t)n;
        return sn * (sn + 1);
    }
}
