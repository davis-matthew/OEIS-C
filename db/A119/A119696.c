/*
 * A119696  a(n) = Fibonacci(n)*n^3*(binomial(2*n, n))^2/(n+1).
 * https://oeis.org/A119696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n) * n^3 * binomial(2n,n)^2 / (n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    /* a(0) = 0 by the definition of Fibonacci(0) = 0. */
    if (n == 0) return (seq_t)0;

    idx_t two_n = n + n;                /* 2*n, safe for n up to 10000 */
    seq_t binom = binomial(two_n, n);   /* binomial(2n,n) */
    seq_t cat   = catalan(n);           /* Catalan(n) = binom/(n+1) */

    seq_t prod = binom * cat;           /* binom^2/(n+1) */
    seq_t f = fib(n);                   /* Fibonacci(n) */
    prod = prod * f;                    /* multiply by Fibonacci(n) */

    /* compute n^3 */
    seq_t n3 = (seq_t)n;
    n3 = n3 * (seq_t)n;
    n3 = n3 * (seq_t)n;

    prod = prod * n3;                   /* final multiplication by n^3 */

    return prod;
}
