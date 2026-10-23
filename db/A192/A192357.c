/*
 * A192357  Constant term of the reduction of the polynomial
 *          p(n,x)=(1/2)((x+3)^n+(x-3)^n) by x^2->x+1.
 * https://oeis.org/A192357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+19*a(n-2)-20*a(n-3)-55*a(n-4); initial values:
 *            a(1)=1, a(2)=0, a(3)=10, a(4)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 10;
    if (n == 4) return 1;

    seq_t a1 = 1, a2 = 0, a3 = 10, a4 = 1, ak;
    for (idx_t i = 5; i <= n; ++i) {
        ak = (seq_t)2 * a4 + (seq_t)19 * a3 - (seq_t)20 * a2 - (seq_t)55 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ak;
    }
    return a4;
}
