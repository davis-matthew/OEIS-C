/*
 * A054254  a(n) is n plus the minimum of the a(i)*a(n-i) of the previous i =
 *          1..n-1.
 * https://oeis.org/A054254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(3)=5, a(n) = ((n*n + n) / 2) - 2 for
 *            all n >= 4 (equivalently a(n) = (n^2 + n - 4)/2 for n>3)
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2u)          return (seq_t)n;
    if (n == 3u)          return (seq_t)5;

    seq_t nn = (seq_t)n;
    seq_t part = (nn * (nn + 1)) / 2;   /* = n*(n+1)/2  */
    return part - 2;                    /* = (n^2 + n - 4)/2 */
}
