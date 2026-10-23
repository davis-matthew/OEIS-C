/*
 * A046978  Numerators of Taylor series for exp(x)*sin(x).
 * https://oeis.org/A046978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = sign(cos(pi*(n-2)/4))
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 8;
    if (r == 0 || r == 4)
        return (seq_t)0;
    if (r <= 3)
        return (seq_t)1;
    return (seq_t)-1;
}
