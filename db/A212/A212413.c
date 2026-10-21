/*
 * A212413  Anchored partitions of a circle.
 * https://oeis.org/A212413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(n) = (2*n-1)*a(n-1) for n >= 3
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    /* a(3) = 9 */
    seq_t result = (seq_t)9;
    for (idx_t i = 4; i <= n; ++i)
    {
        result *= (seq_t)(2 * i - 1);
    }
    return result;
}
