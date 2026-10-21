/*
 * A052554  Expansion of e.g.f.: (1-x)/(1 - x - x^2).
 * https://oeis.org/A052554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*Fibonacci(n-1) for n>=1 with a(0)=1, a(1)=0.
 *            Fibonacci(0)=0, Fibonacci(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)0;
    }

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute Fib(n-1) */
    seq_t fib_prev = (seq_t)0; /* Fib(0) */
    seq_t fib_cur  = (seq_t)1; /* Fib(1) */
    /* If n-1 >= 2, iterate. */
    for (idx_t i = 2; i <= n - 1; ++i) {
        seq_t next = fib_cur + fib_prev;
        fib_prev = fib_cur;
        fib_cur  = next;
    }

    /* Result = n! * Fib(n-1) */
    return fact * fib_cur;
}
