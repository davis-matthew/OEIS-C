/*
 * A022374  Fibonacci sequence beginning 2, 24.
 * https://oeis.org/A022374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) with a(0) = 2, a(1) = 24
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)24;

    seq_t prevPrev = (seq_t)2; /* a(0) */
    seq_t prev     = (seq_t)24;/* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prevPrev + prev; /* a(i) */
        prevPrev = prev;
        prev = cur;
    }

    return cur;
}
