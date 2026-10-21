/*
 * A095122  a(n) = Fibonacci(n)*(2*Fibonacci(n)-1).
 * https://oeis.org/A095122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n) * (2*Fibonacci(n) - 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for idx_t, seq_t definitions */

seq_t a(idx_t n)
{
    /* Fibonacci numbers using a simple two‑variable recurrence.
       This is correct for all n >= 0 and keeps the memory usage minimal. */
    seq_t f_prev = 0; /* Fibonacci(0) */
    seq_t f_curr = 1; /* Fibonacci(1) */

    if (n == 0)
        return 0;           /* a(0) = 0 * (2*0-1) = 0 */

    /* If n == 1 the loop is skipped and f_curr == 1. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* After the loop, f_curr == Fibonacci(n). */
    seq_t fn      = f_curr;
    seq_t twoFn   = fn + fn;          /* 2*Fibonacci(n) */
    seq_t inner   = twoFn - 1;        /* 2*Fibonacci(n) - 1 */

    return fn * inner;                /* Fibonacci(n) * (2*Fibonacci(n)-1) */
}
