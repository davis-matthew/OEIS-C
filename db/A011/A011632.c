/*
 * A011632  28th cyclotomic polynomial.
 * https://oeis.org/A011632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = 0 if n is odd or n > 12; otherwise a(n) = (-1)^(n/2)
 * Range:     i8 n <= 12, i16 n <= 12, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 12ULL) return 0;
    if (n & 1ULL) return 0;
    seq_t k = (seq_t)(n >> 1);   /* k = n/2  (fits into seq_t for n <= 12) */
    return (k & 1) ? -1 : 1;
}
