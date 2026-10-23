/*
 * A103749  Expansion of x*(1+2*x)/(1+x+x^2-2*x^3).
 * https://oeis.org/A103749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1)-a(n-2)+2*a(n-3); a(0)=0, a(1)=1, a(2)=1
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;

    /* Trail of the last three computed values:
       x = a(n-3), y = a(n-2), z = a(n-1) */
    seq_t x = 0; /* a(0) */
    seq_t y = 1; /* a(1) */
    seq_t z = 1; /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = -z - y + (seq_t)2 * x; /* a(i) = -a(i-1)-a(i-2)+2*a(i-3) */
        x = y;            /* shift the window forward */
        y = z;
        z = next;
    }
    return z; /* z now holds a(n) */
}
