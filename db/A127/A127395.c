/*
 * A127395  Product((2*i)!, i = 1..n) - ((n+1)!)^n.
 * https://oeis.org/A127395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product((2*i)!, i = 1..n) - ((n+1)!)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 28 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Handle trivial small cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;

    /* prod = ∏_{i=1..n} (2i)!    fact = current factorial(t)    */
    seq_t prod = (seq_t)1;
    seq_t fact = (seq_t)1;

    idx_t limit = 2 * n;
    for (idx_t t = 1; t <= limit; ++t) {
        /* Update factorial for t */
        fact *= (seq_t)t;
        /* When t is even, t = 2*i, multiply prod by (2i)! */
        if ((t & 1ULL) == 0) {
            prod *= fact;
        }
    }

    /* Compute ((n+1)!)^n */
    seq_t fact_n_plus_1 = factorial(n + 1);
    seq_t pow_term = ipow(fact_n_plus_1, (seq_t)n);

    /* Apply theorem: prod > pow_term for n≥2, but guard against underflow */
    if (prod < pow_term) return (seq_t)0;

    return prod - pow_term;
}
