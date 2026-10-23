/*
 * A180324  Vassiliev invariant of fourth order for the torus knots.
 * https://oeis.org/A180324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)^2/6
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 0; a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    /* convert the index to seq_t once, all further work is in seq_t */
    seq_t a_n = (seq_t)n;          /* a_n = n            */
    seq_t b_n = a_n + 1;           /* b_n = n+1         */
    seq_t c   = (seq_t)2 * a_n + 1;/* c   = 2*n+1       */
    seq_t c1  = c;                 /* first copy of the factor (2*n+1) */
    seq_t c2  = c;                 /* second copy              */

    /* divide the product by 2 (always possible because n*(n+1) is even) */
    if (a_n % 2 == 0)
        a_n /= 2;
    else
        b_n /= 2;

    /* divide the product by 3 (exactly one of the three factors is multiple
       of 3 for any integer n) */
    if (a_n % 3 == 0)
        a_n /= 3;
    else if (b_n % 3 == 0)
        b_n /= 3;
    else
        c1 /= 3; /* divide one copy of (2*n+1) by 3 */

    /* now the product a_n * b_n * c1 * c2 equals the required value */
    return a_n * b_n * c1 * c2;
}
