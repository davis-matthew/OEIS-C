/*
 * A152891  a(1) = b(1) = 0; for n > 1, b(n) = b(n-1) + n-1 + a(n-1) and a(n)
 *          = a(n-1) + n-1 + b(n).
 * https://oeis.org/A152891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+1) - n - 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* Protection against out‑of‑range values */
    seq_t fn = fib(2 * n + 1);          /* Fibonacci(2*n+1) */
    return fn - ((seq_t)n + 1);         /* Subtract n+1 to obtain a(n) */
}
