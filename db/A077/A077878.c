/*
 * A077878  Expansion of (1-x)^(-1)/(1-x+2*x^2+2*x^3).
 * https://oeis.org/A077878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   G(x) = (1-x)^(-1)/(1-x+2*x^2+2*x^3) Recurrence: a(0)=1, a(1)=2,
 *            a(2)=1, a(3)=-4, a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-4) for n ≥ 4
 * Range:     i8 n <= 8, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small cases */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 1;
    if (n == 3) return -4;

    /* start from the known 4th order recurrence */
    seq_t a0 = 1,    /* a(n-4)  */
          a1 = 2,    /* a(n-3)  */
          a2 = 1,    /* a(n-2)  */
          a3 = -4;   /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - 3*a(i-2) + 2*a(i-4) */
        next = 2 * a3 - 3 * a2 + 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
