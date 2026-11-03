/*
 * A206592  E.g.f.: Sum_{n>=0} x^(n^2) * exp(n*x).
 * https://oeis.org/A206592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊√n⌋} ( n! / (n–k²)! ) * k^(n–k²)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 422 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

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
    seq_t fact_k2 = 1;          /* (k²)!  for current k */
    idx_t  prev_k2 = 0;         /* previous k² */

    for (idx_t k = 0; ; ++k) {
        idx_t k2 = k * k;
        if (k2 > n) break;

        /* update factorial(k²) incrementally */
        for (idx_t t = prev_k2 + 1; t <= k2; ++t) {
            fact_k2 *= (seq_t)t;
        }
        prev_k2 = k2;

        idx_t exp = n - k2;
        seq_t powk;

        if (k == 0) {
            /* 0^exp : 1 when exp==0, else 0 */
            powk = (exp == 0) ? 1 : 0;
        } else {
            powk = ipow_idx((seq_t)k, exp);
        }

        if (powk == 0) continue;        /* term contributes nothing */

        seq_t bin   = binomial(n, k2);
        seq_t term  = bin * fact_k2;
        term *= powk;
        sum += term;
    }

    return sum;
}
