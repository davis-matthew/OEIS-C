/*
 * A021124  Expansion of 1/((1-x)*(1-2*x)*(1-5*x)*(1-7*x)).
 * https://oeis.org/A021124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 73*a(n-2) + 129*a(n-3) - 70*a(n-4) (n > 3)
 *            a(0) = 1, a(1) = 15, a(2) = 152, a(3) = 1314
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)152;
    if (n == 3) return (seq_t)1314;

    seq_t v0 = 1, v1 = 15, v2 = 152, v3 = 1314;

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t v4 = 15 * v3 + 129 * v1;   /* 15*a(n-1) + 129*a(n-3) */
        v4 -= 73 * v2;                   /* -73*a(n-2) */
        v4 -= 70 * v0;                   /* -70*a(n-4) */
        v0 = v1; v1 = v2; v2 = v3; v3 = v4;
    }
    return v3;
}
