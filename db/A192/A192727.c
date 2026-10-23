/*
 * A192727  a(n) = Fibonacci(n-2) + 2*a(n-2) - (n mod 2).
 * https://oeis.org/A192727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n-2) + 2*a(n-2) - (n mod 2) with a(0) = a(1) =
 *            0. Computed by a simple O(n) loop
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;

    /* Fibonacci: F0 = 0, F1 = 1 */
    seq_t fib_prev2 = 0; /* F(i-2) */
    seq_t fib_prev1 = 1; /* F(i-1) */

    /* a values: a_prev2 = a(i-2), a_prev1 = a(i-1) */
    seq_t a_prev2 = 0;   /* a0 */
    seq_t a_prev1 = 0;   /* a1 */
    seq_t a_n = 0;       /* current a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = F(i-2) + 2*a(i-2) - (i mod 2) */
        a_n = fib_prev2 + 2 * a_prev2 - (seq_t)(i & 1);

        /* shift a-prev values for the next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_n;

        /* compute next Fibonacci number */
        seq_t fib_curr = fib_prev2 + fib_prev1;  /* F(i) */
        fib_prev2 = fib_prev1;                  /* shift for next i */
        fib_prev1 = fib_curr;
    }

    return a_prev1; /* a(n) */
}
