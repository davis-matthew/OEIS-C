/*
 * A115590  a(0) = 0; a(n) = (1+a(n-1))^3 for n > 0.
 * https://oeis.org/A115590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; a(n) = (1 + a(n-1))^3 for n > 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    if (n == 0)
        return val;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t tmp = val + 1;
        val = tmp * tmp * tmp;
    }
    return val;
}
