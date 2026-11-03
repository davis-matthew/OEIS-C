/*
 * A055682  a(n) = floor(n*sqrt(n)) - sigma(n), where sigma(n) is the sum of
 *          the divisors of n (A000203).
 * https://oeis.org/A055682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(n*sqrt(n)) - sigma(n)
 * Range:     i8 n <= 3, i16 n <= 25, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* compute floor(n*sqrt(n)) as floor(sqrt(n^3)) */
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t flt = floor_sqrt(n3);

    /* compute sigma(n) = sum of all positive divisors of n */
    seq_t sigma = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t d = n / i;
            if (d != i)
                sigma += (seq_t)d;
        }
    }

    return flt - sigma;
}
