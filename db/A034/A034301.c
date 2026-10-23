/*
 * A034301  a(n) = n-th quintic factorial number divided by 4.
 * https://oeis.org/A034301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (Product_{j=1}^{n} (5*j-1))/4 Recurrence: a(1)=1; a(n) =
 *            (5*n-1)*a(n-1) (n>=2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        prod *= (seq_t)(5 * i - 1);
    }
    return prod / 4;
}
