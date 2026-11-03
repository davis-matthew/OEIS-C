/*
 * A135741  E.g.f.: A(x) = Sum_{n>=0} exp(Fibonacci(n)*x) * x^n/n!.
 * https://oeis.org/A135741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * Fibonacci(k)^(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 123 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

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
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t pow;
        if (k == 0) {
            /* Fibonacci(0) is 0. 0^0 must be 1, otherwise 0. */
            pow = (n == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            pow = ipow(fib(k), (seq_t)(n - k));
        }
        sum += binomial(n, k) * pow;
    }
    return sum;
}
