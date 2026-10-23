/*
 * A014506  Inverse of 497th cyclotomic polynomial.
 * https://oeis.org/A014506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if (n mod 497) ∈ [0,6], -1 if (n mod 497) ∈ [71,77], 0
 *            otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 497;
    if (r <= 6) return 1;
    if (r <= 70) return 0;
    if (r <= 77) return -1;
    return 0;
}
