/*
 * A187307  Hankel transform of an alternating sum of Motzkin numbers.
 * https://oeis.org/A187307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor(n/3) * (1 if n%3==0 else 3*floor(n/3)+2)
 * Range:     i8 n <= 126, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    seq_t sign = ((k & 1) ? (seq_t)-1 : (seq_t)1);
    if (n % 3 == 0)
        return sign;          /* gives +1 or -1 */
    else
    {
        seq_t val = (seq_t)(3 * k + 2);
        return sign * val;    /* ±(3k+2) */
    }
}
