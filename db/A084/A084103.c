/*
 * A084103  Expansion of (1+x)^3/(1+x^3).
 * https://oeis.org/A084103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1; for n > 0, a(n) = 3 * (n^2 mod 3) * (-1)^floor(n/3)
 *            Since n^2 mod 3 is 0 when n % 3 == 0 and 1 otherwise, the
 *            sequence is: 1, 3, 3, 0, -3, -3, 0, 3, 3, 0, …
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t r = n % 3;                /* n mod 3 */
    if (r == 0) return (seq_t)0;    /* multiples of 3 give 0 */

    /* floor(n/3) determines the sign */
    idx_t k = n / 3;                /* floor division */
    seq_t val = (seq_t)3;
    if (k & 1)                      /* if k is odd, flip sign */
        val = -val;
    return val;
}
