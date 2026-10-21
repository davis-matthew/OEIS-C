/*
 * A097818  Expansion of (8+10*x)/((1-x)*(1-100*x)).
 * https://oeis.org/A097818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 101*a(n-1) – 100*a(n-2) with a(0) = 8, a(1) = 818
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)8;
    if (n == 1u) return (seq_t)818;

    seq_t prev2 = (seq_t)8;          /* a(0)   */
    seq_t prev1 = (seq_t)818;        /* a(1)   */
    seq_t cur;

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* cur = 101*prev1 - 100*prev2 */
        cur = prev1 * (seq_t)101 - prev2 * (seq_t)100;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
