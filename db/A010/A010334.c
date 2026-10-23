/*
 * A010334  Maximal size of binary code of length n and asymmetric distance 4.
 * https://oeis.org/A010334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Fibonacci(n-7) + 2 (for n ≥ 7), with initial values
 *            a(1) = a(2) = a(3) = 1, a(4) = a(5) = a(6) = a(7) = 2. Here
 *            Fibonacci(0)=0, Fibonacci(1)=1
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include no additional headers; seq_t and idx_t are provided by the build */

seq_t a(idx_t n)
{
    /* Small indices: use the given initial values. */
    if (n <= 3)            /* a(1) ... a(3) */
        return (seq_t)1;
    if (n <= 7)            /* a(4) ... a(7) */
        return (seq_t)2;

    /* For n ≥ 8 we use the closed formula: 2*Fibonacci(n-7)+2. */
    idx_t k = n - 7;       /* index of Fibonacci number */

    /* Compute Fibonacci(k) iteratively using only seq_t operations. */
    seq_t fib_prev = (seq_t)0;   /* Fibonacci(0) */
    seq_t fib_curr = (seq_t)1;   /* Fibonacci(1) */

    if (k == 0) {
        /* fib_prev is already 0 */
    } else if (k == 1) {
        /* fib_curr is already 1 */
    } else {
        for (idx_t i = 2; i <= k; ++i) {
            seq_t fib_next = fib_prev + fib_curr; /* Fibonacci addition */
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
    }

    /* Result is 2 * fib_curr + 2.  Use addition only. */
    seq_t result = fib_curr + fib_curr; /* 2 * fib_curr */
    result += (seq_t)2;                /* +2 */

    return result;
}
