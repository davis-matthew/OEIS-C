/*
 * A215931  Product of Fibonacci and Catalan numbers: a(n) =
 *          A000045(2*n+2)*A000108(n).
 * https://oeis.org/A215931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*n+2) * Catalan(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 951 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    /* compute Fibonacci(2*n+2) and Catalan(n), then take the product */
    seq_t fib_val = fib(2*n + 2);
    seq_t cat_val = catalan(n);
    return fib_val * cat_val;
}
