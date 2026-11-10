/*
 * A054074  Position of n-th 1 in A054073.
 * https://oeis.org/A054074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 + 1 + floor(n*sqrt(2)) - n, with
 *            floor(n*sqrt(2)) = isqrt(2*n^2). The 1 in block p(n) comes after
 *            every h <= n with {h*sqrt(2)} < {sqrt(2)}. Those are the h where
 *            floor(h*sqrt(2)) - floor((h-1)*sqrt(2)) = 2, and there are
 *            floor(n*sqrt(2)) - n of them
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* floor(n*sqrt(2)) computed exactly in idx_t (fine for n < 3e9) */
    idx_t f = isqrt((idx_t)2 * n * n);
    idx_t extra = f - n;              /* number of h <= n with {h*sqrt2} < {sqrt2} */

    /* T(n-1) = n*(n-1)/2, halving first so the product never exceeds the result */
    seq_t tri;
    if ((n & 1) == 0)
        tri = (seq_t)(n / 2) * (seq_t)(n - 1);
    else
        tri = (seq_t)n * (seq_t)((n - 1) / 2);

    return tri + (seq_t)1 + (seq_t)extra;
}
