/*
 * A028253  n mod Fibonacci(n).
 * https://oeis.org/A028253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod Fibonacci(n)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 5) {
        /* Compute small Fibonacci numbers and take n mod F(n). */
        seq_t fib_prev = 1;
        seq_t fib_curr = 1;

        if (n == 1 || n == 2) {
            seq_t fib = 1;
            return ((seq_t)n) % fib;
        }

        for (idx_t i = 3; i <= n; ++i) {
            seq_t fib_next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
        return ((seq_t)n) % fib_curr;
    }

    /* For n > 5, Fibonacci(n) > n, so the remainder is n itself. */
    return (seq_t)n;
}
