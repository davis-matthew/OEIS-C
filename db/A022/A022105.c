/*
 * A022105  Fibonacci sequence beginning 1, 15.
 * https://oeis.org/A022105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2) for n>=2, a(0)=1, a(1)=15
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)15;

    seq_t prev = (seq_t)1;
    seq_t cur  = (seq_t)15;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + cur;
        prev = cur;
        cur  = next;
    }
    return cur;
}
