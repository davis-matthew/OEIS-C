/*
 * A083686  a(0) = 8; for n>0, a(n) = 2*a(n-1) - 1.
 * https://oeis.org/A083686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 1, a(0) = 8 * (Equivalently: a(n) = 7*2^n + 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 8;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = (val << 1);     /* multiply by 2 */
        val -= 1;             /* subtract 1 */
    }
    return val;
}
