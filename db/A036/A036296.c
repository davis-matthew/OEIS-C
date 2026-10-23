/*
 * A036296  Denominator of Sum_{i=1..n} i/2^i.
 * https://oeis.org/A036296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2^(n - v2(n+2)), where v2(x) is the exponent of 2 in x
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes the denominator of  Σ_{i=1..n} i/2^i  (OEIS A036296) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* a(0) = 1 */

    /* Find the exponent of 2 dividing (n+2). */
    idx_t m = n + 2;
    idx_t t = 0;
    while ((m & 1ULL) == 0ULL) {
        ++t;
        m >>= 1;
    }

    /* a(n) = 2^(n - t) */
    idx_t exp = n - t;
    return pow2(exp);
}
