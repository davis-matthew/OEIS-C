/*
 * A192422  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=5 a(n) =
 *            a(n-1)+3*a(n-2)-a(n-3)-a(n-4) for n>=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 5;

    seq_t a0 = 0;   /* a(n-4)   */
    seq_t a1 = 1;   /* a(n-3)   */
    seq_t a2 = 1;   /* a(n-2)   */
    seq_t a3 = 5;   /* a(n-1)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = a3 + (seq_t)3 * a2 - a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
