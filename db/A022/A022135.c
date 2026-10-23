/*
 * A022135  Fibonacci sequence beginning 4, 19.
 * https://oeis.org/A022135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2), a(0)=4, a(1)=19
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    if (n == 1)
        return (seq_t)19;
    seq_t prev = (seq_t)4;
    seq_t curr = (seq_t)19;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
