/*
 * A014026  Inverse of 17th cyclotomic polynomial.
 * https://oeis.org/A014026
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

/* Periodic sequence A014026: inverse of the 17th cyclotomic polynomial.
   a(n) = 1  if n % 17 == 0
   a(n) = -1 if n % 17 == 1
   a(n) = 0  otherwise */
seq_t a(idx_t n)
{
    /* The period is 17, so we only need the remainder modulo 17. */
    idx_t r = n % 17;
    if (r == 0)
        return (seq_t)1;
    if (r == 1)
        return (seq_t)-1;
    return (seq_t)0;
}
