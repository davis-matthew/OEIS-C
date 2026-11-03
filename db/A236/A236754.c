/*
 * A236754  Number of irregular spirals (non-expanded loop) appearing with n
 *          center points.
 * https://oeis.org/A236754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n! if n is odd; * a(n) = n! * (n/4)^2 / ((n-1)*(n-3)) if
 *            n ≡ 0 (mod 4); * a(n) = 0 if n ≡ 2 (mod 4)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    if (n < 1)
        return 0;

    /* odd n: return n! */
    if (n & 1)
        return factorial(n);

    /* even but n % 4 == 2 : zero */
    if ((n & 3) == 2)
        return 0;

    /* n % 4 == 0 : compute n! * (n/4)^2 / ((n-1)*(n-3)) */
    seq_t fac = factorial(n);

    /* denominator = (n-1)*(n-3) */
    seq_t denom = (seq_t)(n - 1) * (seq_t)(n - 3);

    /* first divide factorial by denom (exact division) */
    seq_t res = fac / denom;

    /* multiply by (n/4)^2 */
    idx_t q = n >> 2; /* n/4 */
    seq_t mult = (seq_t)q * (seq_t)q;
    res = res * mult;

    return res;
}
