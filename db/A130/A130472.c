/*
 * A130472  A permutation of the integers: a(n) = (-1)^n * floor( (n+1)/2 ).
 * https://oeis.org/A130472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * floor((n+1)/2)
 * Range:     i8 n <= 255, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(2m) = m;  a(2m+1) = -(m+1) */
    idx_t m = n >> 1;   /* floor(n/2) */
    if ((n & 1) == 0)
        return (seq_t)m;
    else
        return (seq_t)(-((seq_t)(m + 1)));
}
