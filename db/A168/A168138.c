/*
 * A168138  a(n) = Fibonacci(n+1)^tau(n).
 * https://oeis.org/A168138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+1)^τ(n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 23,
 *            big any n
 * Helpers:   fib, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility headers provided by the build system */
#include "../utils/fib.h"         /* seq_t fib(idx_t n); */
#include "../utils/seq_one.h"     /* seq_t seq_one(void); */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b); */

/* Compute the number of divisors τ(n) of n (exclusive of 0). */
static idx_t divisor_count(idx_t n)
{
    idx_t tau = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (m > 1)          /* m is a prime larger than sqrt(n) */
        tau *= 2;

    return tau;
}

/* Main sequence function: a(n) = Fibonacci(n+1)^τ(n) */
seq_t a(idx_t n)
{
    /* Fibonacci index is n+1 (F_0 = 0, F_1 = 1). */
    seq_t fib_val = fib(n + 1);

    /* Calculate τ(n) – the number of divisors of n. */
    idx_t tau = divisor_count(n);

    /* Compute fib_val ^ tau using repeated multiplication. */
    seq_t result = seq_one();
    for (idx_t i = 0; i < tau; ++i)
        result = seq_mul(result, fib_val);

    return result;
}
