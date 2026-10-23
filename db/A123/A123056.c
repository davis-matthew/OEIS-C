/*
 * A123056  Values of X satisfying the equation (X-Y)^4 - 2*X*Y = 0 with
 *          integer X >= Y >= 0.
 * https://oeis.org/A123056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c(n)*(d(n)+1), where c(0)=0, c(1)=1,
 *            c(n)=6*c(n-1)-c(n-2); d(0)=1, d(1)=3, d(n)=6*d(n-1)-d(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t c0 = 0, c1 = 1;
    seq_t d0 = 1, d1 = 3;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t nc = (seq_t)6 * c1 - c0;
        seq_t nd = (seq_t)6 * d1 - d0;
        c0 = c1;
        c1 = nc;
        d0 = d1;
        d1 = nd;
    }
    return c1 * (d1 + 1);
}
