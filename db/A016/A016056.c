/*
 * A016056  Inverse of 2047th cyclotomic polynomial.
 * https://oeis.org/A016056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 1 if 0 ≤ (n mod 2047) ≤ 22; 0 if 23 ≤ (n mod 2047) ≤ 88;
 *            -1 if 89 ≤ (n mod 2047) ≤ 111; 0 otherwise
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 2047;
    if (r <= 22) return (seq_t)1;
    if (r <= 88) return (seq_t)0;
    if (r <= 111) return (seq_t)-1;
    return (seq_t)0;
}
