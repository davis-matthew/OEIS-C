/*
 * A104522  Expansion of (-1+x+3*x^2-x^3)/((x+1)(3*x-1)(x-1)^2).
 * https://oeis.org/A104522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*3^n + 4*(n+1) - (-1)^n)/8 Recurrence: a(0)=1, a(1)=3,
 *            a(2)=7, a(3)=19, a(n)=4*a(n-1)-2*a(n-2)-4*a(n-3)+3*a(n-4)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)19;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)7;
    seq_t a3 = (seq_t)19;
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        a4 = 4 * a3;
        a4 -= 2 * a2;
        a4 -= 4 * a1;
        a4 += 3 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
