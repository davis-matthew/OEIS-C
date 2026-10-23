/*
 * A158083  a(n) = Fibonacci(n+3) for n < 5 and 9*n - 15 otherwise.
 * https://oeis.org/A158083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+3) for n<5; a(n) = 9*n-15 for n>=5
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        idx_t k = n + 3;
        seq_t prev = 0;
        seq_t cur  = 1;
        if (k == 0) return prev;
        if (k == 1) return cur;
        idx_t i;
        for (i = 2; i <= k; ++i) {
            seq_t next = prev + cur;
            prev = cur;
            cur  = next;
        }
        return cur;
    } else {
        return (seq_t)9 * (seq_t)n - (seq_t)15;
    }
}
