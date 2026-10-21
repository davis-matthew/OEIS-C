/*
 * A077915  Expansion of 1/((1-x)*(1+2*x-2*x^2-2*x^3)).
 * https://oeis.org/A077915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 24, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) + 4*a(n-2) - 2*a(n-4) for n>=4
   with a(0)=1, a(1)=-1, a(2)=5, a(3)=-9 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -1;
    if (n == 2) return 5;
    if (n == 3) return -9;

    seq_t a0 = 1;      /* a(n-4) */
    seq_t a1 = -1;     /* a(n-3) */
    seq_t a2 = 5;      /* a(n-2) */
    seq_t a3 = -9;     /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = -a3 + 4 * a2 - 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
