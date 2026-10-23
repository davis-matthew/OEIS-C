/*
 * A194510  First coordinate of (2,5)-Lagrange pair for n.
 * https://oeis.org/A194510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n - 5*floor((4*n + 2)/7)
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence has offset 1, but a(n) for n=0 is never used.
       To keep the function well defined for n==0, we return 0. */
    if (n == 0)
        return 0;

    /* Compute floor((4*n + 2)/7) without overflow.
       Let n = 7*q + r, 0 <= r < 7.
       Then (4*n + 2)/7 = 4*q + floor((4*r + 2)/7). */
    idx_t q = n / 7;
    idx_t r = n % 7;
    idx_t term = 4 * q + ( (4 * r + 2) / 7 );

    /* Now compute a(n) = 3*n - 5*term. */
    return (seq_t)n * 3 - (seq_t)term * 5;
}
