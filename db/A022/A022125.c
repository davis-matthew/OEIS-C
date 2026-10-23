/*
 * A022125  Fibonacci sequence beginning 3, 14.
 * https://oeis.org/A022125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) with a(0) = 3, a(1) = 14
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)14;

    seq_t prevprev = (seq_t)3;
    seq_t prev     = (seq_t)14;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
