/*
 * A163818  Expansion of (1 - x) * (1 - x^6) / ((1 - x^2) * (1 - x^5)) in
 *          powers of x.
 * https://oeis.org/A163818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163818: Expansion of (1 - x) * (1 - x⁶) / ((1 - x²) * (1 - x⁵))
   Formula: a(0) = 1;
            for n > 0:
              if n % 5 == 0      a(n) = 0;
              if n % 5 == 1 or 3 a(n) = -1;
              else              a(n) = 1;
*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r = n % 5;

    if (r == 0)
        return (seq_t)0;
    if (r == 1 || r == 3)
        return (seq_t)-1;
    return (seq_t)1;
}
