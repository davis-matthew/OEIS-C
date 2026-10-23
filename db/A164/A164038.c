/*
 * A164038  Expansion of (5-19*x)/(1-10*x+23*x^2).
 * https://oeis.org/A164038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2); a(0) = 5, a(1) = 31
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)31;
    seq_t prev2 = (seq_t)5;      /* a(0) */
    seq_t prev1 = (seq_t)31;     /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)10 * prev1 - (seq_t)23 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
