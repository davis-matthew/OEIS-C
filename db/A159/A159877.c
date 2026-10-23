/*
 * A159877  Numerator of Hermite(n, 12/23).
 * https://oeis.org/A159877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=24, a(n+1)=24*a(n)-1058*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (a(n)=23^n*Hermite(n,12/23))                           */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 24;
    /* a0 = a(n-1), a1 = a(n)  */
    seq_t a0 = 1;      /* a(0) */
    seq_t a1 = 24;     /* a(1) */
    for (idx_t i = 1; i < n; ++i) { /* i = current n-1, computing a(n) */
        seq_t prod = (seq_t)i * a0;           /* i * a(n-1)   */
        seq_t term = 24 * a1 - 1058 * prod;   /* a(n+1) formula */
        a0 = a1;
        a1 = term;
    }
    return a1;
}
