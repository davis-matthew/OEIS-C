/*
 * A023483  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 1) and
 *          d(n) = (n-th non-Fibonacci number).
 * https://oeis.org/A023483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n+2) + A001690(n)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   fib, nth_non_fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A023483.
   It uses the supplied fib routine for Fibonacci numbers and
   a simple gap‑counting algorithm for the n‑th non‑Fibonacci number. */

#include "../utils/fib.h"
#include "../utils/nth_non_fibonacci.h"

seq_t
a(idx_t n)
{
    /* n‑th Fibonacci number > 1 is A000045(n+2) */
    seq_t b = fib(n + (idx_t)2);

    /* n‑th non‑Fibonacci number is A001690(n) */
    seq_t d = nth_non_fibonacci(n);

    return b + d;
}
