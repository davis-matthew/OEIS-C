/*
 * A020717  Pisot sequences L(6,9), E(6,9).
 * https://oeis.org/A020717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+5)+1 = A001611(n+5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t target = n + 5;    /* the Fibonacci index */
    seq_t f0 = 0;
    seq_t f1 = 1;
    idx_t i;

    /* Compute fib(target) using iterated addition. */
    for (i = 2; i <= target; ++i) {
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }

    return f1 + 1;  /* a(n) = fib(n+5) + 1 */
}
