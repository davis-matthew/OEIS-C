/*
 * A022399  Fibonacci sequence beginning 1, 29.
 * https://oeis.org/A022399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 29
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)29;
    }
    seq_t prev = (seq_t)1;
    seq_t curr = (seq_t)29;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
