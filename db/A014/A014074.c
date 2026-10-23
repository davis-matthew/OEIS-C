/*
 * A014074  Inverse of 65th cyclotomic polynomial.
 * https://oeis.org/A014074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 65 ∈ {0,1,2,3,4}; * a(n) = –1 if n mod 65 ∈
 *            {13,14,15,16,17}; * otherwise 0. The sequence is periodic with
 *            period 65
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the remainder modulo 65 */
    const idx_t r = n % 65;
    if (r <= 4)
        return (seq_t)1;
    if (r >= 13 && r <= 17)
        return (seq_t)-1;
    return (seq_t)0;
}
