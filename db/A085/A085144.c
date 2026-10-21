/*
 * A085144  a(0)=0, a(2n) = a(n)+1, a(2n+1) = -a(n).
 * https://oeis.org/A085144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0; for n > 0, if n is even a(n) = a(n/2) + 1, if n is odd
 *            a(n) = -a((n-1)/2)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((n & 1) == 0)            /* n even */
        return a(n >> 1) + 1;
    else                         /* n odd  */
        return -a((n - 1) >> 1);
}
