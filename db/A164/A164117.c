/*
 * A164117  Expansion of (1 - x) * (1 - x^10) / ((1 - x^2) * (1 - x^4) * (1 -
 *          x^5)) in powers of x.
 * https://oeis.org/A164117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164117 (OEIS): Expansion of (1 - x) * (1 - x^10) / ((1 - x^2) * (1 - x^4) * (1 - x^5)).
   The generating function simplifies to (1 - x + x^2 - x^3 + x^4) / (1 - x^4), giving
   a(0) = 1; for n > 0,
   a(n) = -1 if n mod 4 is 1 or 3,
          1  if n mod 4 is 2,
          2  if n mod 4 is 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t r = n & 3u;          /* n modulo 4 (n is unsigned) */
    if (r == 1 || r == 3) return (seq_t)-1;
    if (r == 2)              return (seq_t)1;
    /* r == 0 for n > 0 */
    return (seq_t)2;
}
