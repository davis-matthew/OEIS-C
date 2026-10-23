/*
 * A014988  Inverse of 979th cyclotomic polynomial.
 * https://oeis.org/A014988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 979 ∈ [0,10]; -1 if n mod 979 ∈ [89,99]; 0
 *            otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % (idx_t)979;  /* period length */
    if (r <= 10) return (seq_t)1;
    if (r <= 88) return (seq_t)0;
    if (r <= 99) return (seq_t)(-1);
    return (seq_t)0;
}
