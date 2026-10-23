/*
 * A033889  a(n) = Fibonacci(4*n + 1).
 * https://oeis.org/A033889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n + 1) Recurrence: a(n) = 7*a(n-1) - a(n-2),
 *            a(0) = 1, a(1) = 5 This implementation uses fast‑doubling to
 *            compute the Fibonacci number efficiently for arbitrary n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib_pair_doubling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair_doubling.h"

seq_t a(idx_t n)
{
    /* We need Fib(4*n + 1). */
    idx_t m = 4 * n + 1;
    seq_t fn, fn1;
    fib_pair_doubling(m, &fn, &fn1);
    return fn;
}
