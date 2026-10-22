/*
 * A004091  Fibonacci numbers written backwards.
 * https://oeis.org/A004091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000045(n))
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 47, u64 n <= 92, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_digits(seq_t num) {
    seq_t reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + (num % 10);
        num /= 10;
    }
    return reversed;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    return reverse_digits(fib_curr);
}
