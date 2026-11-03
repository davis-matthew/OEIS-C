/*
 * A037960  a(n) = n*(3*n+1)*(n+2)!/24.
 * https://oeis.org/A037960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n+1)*(n+2)!/24
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 284 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

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
    /* compute (n+2)! */
    seq_t fact = factorial(n + 2);

    /* compute n*(3*n+1) in a way that avoids overflow in idx_t */
    seq_t nSeq = (seq_t)n;
    seq_t term = nSeq;          /* start with n */
    term *= (seq_t)3;           /* n*3 */
    term += (seq_t)1;           /* n*3 + 1 */
    term *= nSeq;               /* n*(3*n+1) */

    /* cancel factors of 24 from term and factorial separately */
    seq_t g1 = gcd(term, (seq_t)24);
    term /= g1;
    seq_t den = (seq_t)24 / g1;

    seq_t g2 = gcd(fact, den);
    fact /= g2;
    den /= g2;                  /* den should now be 1 */

    /* multiply the reduced parts */
    seq_t result = fact * term;

    /* final division if any denominator remains (safety) */
    if (den != (seq_t)1)
        result /= den;

    return result;
}
