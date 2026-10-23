/*
 * A100059  First differences of A052911.
 * https://oeis.org/A100059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=5, a(3)=14, a(n)=3*a(n-1)+a(n-2)-2*a(n-3) (n>=4)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset not defined, safe guard */
    if (n == 1) return 1;
    if (n == 2) return 5;
    if (n == 3) return 14;

    seq_t a1 = 1, a2 = 5, a3 = 14, ai;
    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)3 * a3 + a2 - (seq_t)2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
