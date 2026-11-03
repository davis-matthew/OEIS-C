/*
 * A122880  Catalan numbers minus odd-indexed Fibonacci numbers.
 * https://oeis.org/A122880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000108(n) - A001519(n) = catalan(n) - fib(2*n-1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* a(0) is undefined for the OEIS sequence (offset 1).  The
       function is only called with n >= 1.  For safety, return 0
       for n == 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t cat = catalan(n);
    idx_t fib_index = (n << 1) - 1;      /* 2*n - 1 */
    seq_t fib_val = fib(fib_index);

    return cat - fib_val;
}
