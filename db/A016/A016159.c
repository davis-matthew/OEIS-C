/*
 * A016159  Expansion of 1/((1-4*x)*(1-12*x)).
 * https://oeis.org/A016159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + 4^n, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    seq_t value   = (seq_t)1;   /* a(0) */
    seq_t pow4    = (seq_t)4;   /* 4^1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        value = (seq_t)12 * value + pow4;   /* a(i) = 12*a(i-1) + 4^i */
        pow4  = pow4 * (seq_t)4;           /* next power: 4^(i+1) */
    }
    return value;
}
