/*
 * A157194  Fibonacci sequence beginning 41, 43.
 * https://oeis.org/A157194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 41, a(1) = 43
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   return (seq_t)41;
    if (n == 1)   return (seq_t)43;

    seq_t prev = (seq_t)41;   /* a(0) */
    seq_t curr = (seq_t)43;   /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = prev + curr;   /* a(i) = a(i-1) + a(i-2) */
        prev = curr;
        curr = next;
    }
    return curr;   /* a(n) */
}
