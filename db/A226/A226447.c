/*
 * A226447  Expansion of (1-x+x^3)/(1-x^2+2*x^3-x^4).
 * https://oeis.org/A226447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) - 2*a(n-3) + a(n-4) for n >= 4 Initial values:
 *            a(0)=1, a(1)=-1, a(2)=1, a(3)=-2 Recurrence is derived from the
 *            generating function (1-x+x^3)/(1-x^2+2*x^3-x^4)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -1;
    if (n == 2) return 1;
    if (n == 3) return -2;

    /* Sliding window for a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t a0 = 1;  /* a(0)   */
    seq_t a1 = -1; /* a(1)   */
    seq_t a2 = 1;  /* a(2)   */
    seq_t a3 = -2; /* a(3)   */

    seq_t res = 0;
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = a(i-2) - 2*a(i-3) + a(i-4) */
        res = a2 - 2 * a1 + a0;

        /* slide window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }
    return res;
}
