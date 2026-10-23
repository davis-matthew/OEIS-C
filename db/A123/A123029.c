/*
 * A123029  a(2*n-1) = Product_{i=1..n} Fibonacci(2*i-1) and a(2*n) =
 *          Product_{i=1..n} Fibonacci(2*i).
 * https://oeis.org/A123029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 19, u128 n <= 27,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 1, for n ≥ 3, a(n) = Fibonacci(n) * a(n‑2) */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle inputs below the CF offset; behaviour is not required. */
    if (n <= 1U)
        return (seq_t)1;
    if (n == 2U)
        return (seq_t)1;

    /* a0 holds a(i-2), a1 holds a(i-1) during the loop */
    seq_t a0 = (seq_t)1; /* a(1) */
    seq_t a1 = (seq_t)1; /* a(2) */
    seq_t new_val;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t f = fib(i);          /* Fibonacci(i) */
        new_val = f * a0;          /* a(i) = fib(i) * a(i-2) */
        a0 = a1;                   /* shift window: a(i-2) becomes previous a(i-1) */
        a1 = new_val;              /* a(i) becomes new a(i-1) for next iteration */
    }
    return a1;
}
