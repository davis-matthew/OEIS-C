/*
 * A022101  Fibonacci sequence beginning 1, 11.
 * https://oeis.org/A022101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 11
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t prev = (seq_t)1;
    seq_t cur  = (seq_t)11;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + cur;
        prev = cur;
        cur = next;
    }

    return cur;
}
