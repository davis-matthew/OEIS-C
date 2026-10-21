/*
 * A225000  Duplicate of A047211.
 * https://oeis.org/A225000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor((5*n-1)/2)
 * Range:     i8 n <= 51, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A225000.
   The function assumes n >= 1. */
seq_t a(idx_t n)
{
    /* Convert n to the arithmetic type seq_t before multiplying
       to avoid accidental promotion to a smaller type. */
    return ((seq_t)n * 5 - 1) / 2;
}
