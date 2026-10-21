/*
 * A222182  Numbers m such that 2*m + 11 is a square.
 * https://oeis.org/A222182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^2 - 2*n - 5
 * Range:     i8 n <= 8, i16 n <= 128, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of OEIS A222182 (offset 1). */
seq_t a(idx_t n)
{
    /* Compute using the closed form. The cast to seq_t is required
       to ensure all arithmetic is performed in seq_t type.  */
    seq_t tn = (seq_t)n;
    seq_t res = tn * tn * 2 - tn * 2 - (seq_t)5;
    return res;
}
