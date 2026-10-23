/*
 * A199805  G.f.: 1/(1 + x + 5*x^2 - x^3 + x^4).
 * https://oeis.org/A199805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) - 5*a(n-2) + a(n-3) - a(n-4) initial values:
 *            a(0)=1, a(1)=-1, a(2)=-4, a(3)=10
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 26, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -1;
    if (n == 2) return -4;
    if (n == 3) return 10;

    seq_t a0 = 1, a1 = -1, a2 = -4, a3 = 10, next;
    for (idx_t i = 4; i <= n; ++i)
    {
        next = -a3 - 5 * a2 + a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
