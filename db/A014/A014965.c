/*
 * A014965  a(n) = lcm(n, Fibonacci(n)).
 * https://oeis.org/A014965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, F(n)) where F(n) is the nth Fibonacci number
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

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
    /* Convert index to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;
    if (nseq == 0)
        return (seq_t)0;          /* trivial case (offset starts at 1) */

    /* Compute Fibonacci(n) using the provided helper */
    seq_t fn = fib(n);

    /* Compute GCD of n and Fibonacci(n) */
    seq_t g = gcd(nseq, fn);

    /* lcm(n, fn) = n/g * fn */
    seq_t res = (nseq / g) * fn;

    return res;
}
