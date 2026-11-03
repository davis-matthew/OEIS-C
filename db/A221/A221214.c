/*
 * A221214  O.g.f.: Sum_{n>=0} (3*n+1)^(3*n+1) * exp(-(3*n+1)^3*x) * x^n / n!.
 * https://oeis.org/A221214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/n! * Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) *
 *            (3*k+1)^(3*n+1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 158 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    seq_t sum = 0;
    seq_t exp = (seq_t)(3 * n + 1);

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base   = (seq_t)(3 * k + 1);
        seq_t pow_val = ipow(base, exp);
        seq_t term = binomial(n, k) * pow_val;

        if ((n - k) & 1)      /* (-1)^(n-k) == -1 */
            sum -= term;
        else                  /* (+1) */
            sum += term;
    }

    seq_t fact = factorial(n);
    return sum / fact;        /* exact division */
}
