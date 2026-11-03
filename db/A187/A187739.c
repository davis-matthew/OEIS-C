/*
 * A187739  G.f.: Sum_{n>=0} (3*n+2)^n * x^n / (1 + (3*n+2)*x)^n.
 * https://oeis.org/A187739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n == 0; a(n) = (3*n + 7) * 3^(n-1) * n! / 2 for n >
 *            0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The factorial() and big_pow() helpers are provided by the build. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Factorial of n (big integer if needed) */
    seq_t fact = factorial(n);

    /* 3^(n-1)  (big integer if needed) */
    seq_t pow3 = ipow((seq_t)3, (seq_t)(n - 1));

    /* Combine all factors: fact * pow3 * (3*n + 7) / 2 */
    seq_t result = fact * pow3;
    result = result * (seq_t)(3 * n + 7);
    result = result / 2;

    return result;
}
