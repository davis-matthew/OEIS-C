/*
 * A163810  Expansion of (1 - x) * (1 - x^2) * (1 - x^3) / (1 - x^6) in powers
 *          of x.
 * https://oeis.org/A163810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n==0; a(n) = 0 if n%6==0 or n%6==3; a(n) = -1 if
 *            n%6==1 or n%6==2; a(n) = 1 if n%6==4 or n%6==5
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t r = n % 6;
    if (r == 0 || r == 3)
        return (seq_t)0;
    if (r == 1 || r == 2)
        return (seq_t)-1;
    return (seq_t)1;
}
