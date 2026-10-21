/*
 * A078058  Expansion of (1-x)/(1+2*x-x^2+x^3).
 * https://oeis.org/A078058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2) - a(n-3) a(0) = 1, a(1) = -3, a(2) = 7
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -3;
    if (n == 2) return 7;

    seq_t a0 = 1;   /* a(n-3)   */
    seq_t a1 = -3;  /* a(n-2)   */
    seq_t a2 = 7;   /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = -2*a(i-1) + a(i-2) - a(i-3) */
        seq_t next = -((seq_t)2 * a2) + a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
