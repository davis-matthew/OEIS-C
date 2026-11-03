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
 * Validated: standard, pass, 41 terms through n = 961 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility functions fib() and catalan() from the utils library */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

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
    seq_t f = fib(n + 1);   /* Fibonacci(n+1) */
    seq_t c = catalan(n);   /* Catalan(n) */
    return f * c;           /* product */
}
