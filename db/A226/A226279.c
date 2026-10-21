/*
 * A226279  a(4n) = a(4n+2) = 2*n , a(4n+1) = a(4n+3) = 2*n-1.
 * https://oeis.org/A226279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4n) = a(4n+2) = 2*n ; a(4n+1) = a(4n+3) = 2*n-1
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 4;
    idx_t r = n % 4;

    /* compute 2*m in seq_t to allow a negative result when r==1 or r==3 */
    seq_t t = (seq_t)2 * (seq_t)m;

    if (r == 0 || r == 2)
        return t;
    else
        return t - 1;
}
