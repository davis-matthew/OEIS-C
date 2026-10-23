/*
 * A036545  a(n) = T(4,n), array T given by A048471.
 * https://oeis.org/A036545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 3*a(n-1) + 14 for n >= 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;           /* a(0) */
    for (idx_t i = (idx_t)0; i < n; ++i)
    {
        val = val * (seq_t)3 + (seq_t)14;
    }
    return val;
}
