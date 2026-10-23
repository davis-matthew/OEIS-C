/*
 * A136270  a(n) = 20*a(n-1) - 3*a(n-2).
 * https://oeis.org/A136270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 3*a(n-2), with a(1)=1, a(2)=17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)17;

    seq_t prevprev = (seq_t)1;
    seq_t prev     = (seq_t)17;
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = (20 * prev) - (3 * prevprev);
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
