/*
 * A014824  a(0) = 0; for n>0, a(n) = 10*a(n-1) + n.
 * https://oeis.org/A014824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; for n>0, a(n)=10*a(n-1)+n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n == 0)
        return result;

    for (idx_t i = 1; i <= n; ++i)
    {
        result = result * 10 + (seq_t)i;
    }

    return result;
}
