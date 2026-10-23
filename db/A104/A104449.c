/*
 * A104449  Fibonacci sequence with initial values a(0) = 3 and a(1) = 1.
 * https://oeis.org/A104449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    seq_t prev = (seq_t)3;
    seq_t cur  = (seq_t)1;
    seq_t next;
    for (idx_t i = 2; i <= n; ++i)
    {
        next = prev + cur;
        prev = cur;
        cur  = next;
    }
    return cur;
}
