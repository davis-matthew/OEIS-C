/*
 * A213586  Antidiagonal sums of the convolution array A213584.
 * https://oeis.org/A213586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+8) - (21 + 10*n + 2*n^2)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple iterative Fibonacci; works for any seq_t type. */
seq_t a(idx_t n)
{
    /* Compute Fibonacci(n+8) iteratively.  Fibonacci(0)=0, Fibonacci(1)=1. */
    idx_t k = n + 8;                 /* the Fibonacci index */
    seq_t f0 = 0, f1 = 1;
    if (k >= 2) {
        for (idx_t i = 2; i <= k; ++i) {
            seq_t temp = f0 + f1;
            f0 = f1;
            f1 = temp;
        }
    }
    seq_t fib_val = f1;              /* Fibonacci(k) */

    /* Compute the polynomial 21 + 10*n + 2*n^2. */
    seq_t sn   = (seq_t)n;
    seq_t poly = (seq_t)21;
    poly += (seq_t)10 * sn;
    poly += (seq_t)2 * sn * sn;

    return fib_val - poly;
}
