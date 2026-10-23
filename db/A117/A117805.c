/*
 * A117805  Start with 3. Square the previous term and subtract it.
 * https://oeis.org/A117805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 3, a(n) = a(n-1)^2 - a(n-1) for n > 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 3;                     /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (val - 1);         /* a(i) = a(i-1) * (a(i-1) - 1) */
    }
    return val;
}
