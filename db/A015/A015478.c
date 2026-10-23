/*
 * A015478  Inverse of 1469th cyclotomic polynomial.
 * https://oeis.org/A015478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 1 if n mod 1469 <= 12 -1 if 113 <= n mod 1469 <= 125 0
 *            otherwise
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the coefficient sequence of (x‑1)(1+x+…+x^12)(1+x+…+x^112) */
/* The sequence is periodic with period 1469. */

seq_t a(idx_t n)
{
    const idx_t PERIOD = 1469;
    idx_t r = n % PERIOD;

    if (r <= 12)
        return (seq_t)1;

    if (r >= 113 && r <= 125)
        return (seq_t)-1;

    return (seq_t)0;
}
