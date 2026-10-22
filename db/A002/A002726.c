/*
 * A002726  a(n) = Fibonacci(n+1) mod n.
 * https://oeis.org/A002726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+1) mod n
 * Range:     u8 n <= 268, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    
    seq_t mod = (seq_t)n;
    seq_t fib_a = 1;
    seq_t fib_b = 1;
    
    for (idx_t i = 2; i < n; i++) {
        seq_t temp = (fib_a + fib_b) % mod;
        fib_a = fib_b;
        fib_b = temp;
    }
    
    return (fib_a + fib_b) % mod;
}
