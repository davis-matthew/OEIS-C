/*
 * A022104  Fibonacci sequence beginning 1, 14.
 * https://oeis.org/A022104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 14
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 14;

    seq_t prev_prev = 1;   /* a(0) */
    seq_t prev = 14;       /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = prev_prev + prev;
        prev_prev = prev;
        prev = curr;
    }
    return curr;
}
