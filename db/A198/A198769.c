/*
 * A198769  a(n) = (9*5^n-1)/4.
 * https://oeis.org/A198769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)+1, a(0)=2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t val = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i)
        val = val * (seq_t)5 + (seq_t)1;
    return val;
}
