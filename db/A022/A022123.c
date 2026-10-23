/*
 * A022123  Fibonacci sequence beginning 3, 11.
 * https://oeis.org/A022123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 11
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)11;
    seq_t prev = (seq_t)3;
    seq_t curr = (seq_t)11;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
