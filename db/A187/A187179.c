/*
 * A187179  Number of nontrivial compositions of differential operations and
 *          directional derivative of the n-th order on the space R^10.
 * https://oeis.org/A187179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(n) + 6 (OEIS: Fibonacci(n) + 6)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term directly, using O(n) time and O(1) memory.
   Works for any seq_t supporting + and unary 0/1 constants. */
seq_t a(idx_t n)
{
    /* n = 0 is outside the demanded offset; return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    /* Compute Fibonacci(n) iteratively.  After n iterations `f0` holds F(n). */
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t tmp = f0 + f1;   /* next Fibonacci number */
        f0 = f1;
        f1 = tmp;
    }

    /* a(n) = Fib(n) + 6 */
    return f0 + (seq_t)6;
}
