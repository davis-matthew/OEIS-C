/*
 * A152886  Number of descents beginning and ending with an even number in all
 *          permutations of {1,2,...,n}.
 * https://oeis.org/A152886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * binomial(floor(n/2), 2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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
    /* floor(n/2) is the number of even elements in {1,…,n} */
    idx_t k = n / 2;

    /* For k < 2 the binomial coefficient is zero → a(n) = 0 */
    if (k < 2) {
        return 0;
    }

    /* binomial(k,2) = k*(k-1)/2  (handled by the helper) */
    seq_t bin = binomial(k, 2);

    /* (n-1)! – use the utility that handles arbitrary‐precision */
    seq_t fact = factorial(n - 1);

    /* a(n) = (n-1)! * binomial(k,2) */
    return fact * bin;
}
