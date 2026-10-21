/*
 * A076128  Difference between the product of numbers up to n and the sum of
 *          numbers up to n.
 * https://oeis.org/A076128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n! - n*(n+1)/2
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        fact *= (seq_t)i;
    }

    /* compute the triangular number n*(n+1)/2 (converting n to seq_t first) */
    seq_t tri = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;

    return fact - tri;
}
