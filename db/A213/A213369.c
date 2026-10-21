/*
 * A213369  The twisted Stern sequence: a(0) = 0, a(1) = 1 and a(2n) = -a(n),
 *          a(2n + 1) = -a(n)-a(n + 1) for n>=1.
 * https://oeis.org/A213369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(0) = 0, a(1) = 1, a(2n) = -a(n), a(2n+1) = -a(n) - a(n+1)
 * Range:     i8 n <= 4394, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    if ((n & 1) == 0)          /* n is even: n = 2k */
        return -a(n >> 1);     /* a(2k) = -a(k) */
    else                        /* n is odd: n = 2k+1 */
        return -a(n >> 1) - a((n >> 1) + 1); /* a(2k+1) = -a(k) - a(k+1) */
}
