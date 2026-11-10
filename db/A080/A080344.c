/*
 * A080344  Partial sums of A023969.
 * https://oeis.org/A080344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A023969(k), with A023969(k) = round(sqrt(k))
 *            - floor(sqrt(k)). Closed form: with f = floor(sqrt(n)), a(n) =
 *            f*(f-1)/2 + max(0, n - f^2 - f)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-07)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    idx_t f = isqrt(n);
    seq_t F = (seq_t)f;
    seq_t N = (seq_t)n;
    seq_t res = F * (F - 1) / 2;
    seq_t base = F * F + F;
    if (N > base)
        res += N - base;
    return res;
}
