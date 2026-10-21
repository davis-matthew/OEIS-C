/*
 * A099100  a(n) = Fibonacci(5*n+1).
 * https://oeis.org/A099100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(5*n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the Fibonacci index: k = 5*n+1 */
    idx_t k = n * ((idx_t)5) + ((idx_t)1);
    /* Fast iterative computation of Fib(k) using seq_t arithmetic */
    seq_t prev = 0;   /* Fib(0) */
    seq_t curr = 1;   /* Fib(1) */
    if (k == 0) return prev;
    /* Iterate from i = 1 to k, updating prev and curr accordingly */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    /* After the loop, prev holds Fib(k) */
    return prev;
}
