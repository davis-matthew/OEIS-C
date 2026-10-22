/*
 * A005593  a(n) = (F(2n+1) + F(2n-1) + F(n+3) - 2)/2, where F() = Fibonacci
 *          numbers A000045.
 * https://oeis.org/A005593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(2n+1) + F(2n-1) + F(n+3) - 2)/2, where F() = Fibonacci
 *            numbers
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t fib(idx_t k) {
    if (k == 0) return 0;
    if (k == 1 || k == 2) return 1;
    
    seq_t a = 1, b = 1;
    for (idx_t i = 3; i <= k; i++) {
        seq_t temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

seq_t a(idx_t n) {
    seq_t f1 = fib(2 * n + 1);
    seq_t f2 = fib(2 * n - 1);
    seq_t f3 = fib(n + 3);
    
    seq_t sum = f1 + f2 + f3;
    return (sum - 2) / 2;
}
