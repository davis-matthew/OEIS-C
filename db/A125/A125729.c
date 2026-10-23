/*
 * A125729  Numbers whose base 7 representation is 555....5.
 * https://oeis.org/A125729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 7*a(n-1) + 5, a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 is already the start value */
    if (n <= 1)
        return (seq_t)0;

    seq_t val = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * (seq_t)7 + (seq_t)5;
    }
    return val;
}
