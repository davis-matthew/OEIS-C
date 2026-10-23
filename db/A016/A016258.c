/*
 * A016258  Inverse of 2249th cyclotomic polynomial.
 * https://oeis.org/A016258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if (n mod 2249) < 13, otherwise 0
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic with period 2249, first 13 terms are 1, the rest 0 in each block */

seq_t a(idx_t n)
{
    /* Compute index modulo the period */
    idx_t r = n % 2249ULL;

    /* Return 1 for the first 13 positions, 0 otherwise */
    return (r < 13ULL) ? (seq_t)1 : (seq_t)0;
}
