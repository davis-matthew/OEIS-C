/*
 * A006270  Denominators of a continued fraction for (1 + sqrt(5))/2.
 * https://oeis.org/A006270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(m)*L(2m) = F(4m)/F(m) with m = 3^(n-1), L = Lucas
 *            numbers. Since m is odd, L(2m) = L(m)^2 + 2 and L(3m) = L(m)^3 +
 *            3*L(m). So with x(1) = 1 and x(k+1) = x(k)^3 + 3*x(k), a(n) =
 *            x(n)*(x(n)^2 + 2) for n >= 1, and a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x;
    idx_t k;
    if (n == 0)
        return (seq_t)1;
    x = (seq_t)1;               /* L(3^0) = L(1) = 1 */
    for (k = 1; k < n; k++)
        x = x * x * x + (seq_t)3 * x;   /* L(3m) = L(m)^3 + 3 L(m), m odd */
    return x * (x * x + (seq_t)2);
}
