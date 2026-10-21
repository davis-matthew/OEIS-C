/*
 * A098614  Product of Fibonacci and Catalan numbers: a(n) =
 *          A000045(n+1)*A000108(n).
 * https://oeis.org/A098614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+1) * Catalan(n) (zero‑based index)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   catalan, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility functions fib() and catalan() from the utils library */

#include "../utils/fib.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n + 1);   /* Fibonacci(n+1) */
    seq_t c = catalan(n);   /* Catalan(n) */
    return f * c;           /* product */
}
