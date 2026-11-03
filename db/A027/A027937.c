/*
 * A027937  a(n) = T(2*n, n+1), T given by A027935.
 * https://oeis.org/A027937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+3) - 2*n - 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 997 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided fib(idx_t n) function from ../utils/fib.h */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* For the defined offset a(1) = 1.  n == 0 is outside the
       sequence but handled gracefully. */
    if (n == 0u)
        return (seq_t)0;

    /* Compute Fibonacci value at position 2*n + 3 */
    idx_t fib_index = (n << 1) + 3;   /* 2*n + 3 */
    seq_t fib_val   = fib(fib_index);

    /* Subtract 2*n + 2 from the Fibonacci value */
    seq_t sub = (seq_t)((n << 1) + 2); /* 2*n + 2 */

    return fib_val - sub;
}
