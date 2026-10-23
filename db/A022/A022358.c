/*
 * A022358  Fibonacci sequence beginning 0, 24.
 * https://oeis.org/A022358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*A000045(n)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t prev = 0, curr = 1;
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return (seq_t)24 * curr;
}
