/*
 * A224195  Ordered sequence of numbers of form (2^n - 1)*2^m + 1 where n >=
 *          1, m >= 1.
 * https://oeis.org/A224195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^i-1) * 2^j + 1 * where i = n - t(t+1)/2, * j =
 *            (t^2+3t+4)/2 - n, * t = floor(( -1 + sqrt(8n-7) ) / 2) (n ≥ 1)
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 496, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* pow2(idx_t) → seq_t */

/* integer square root for 64‑bit unsigned integers, floor(sqrt(x)) */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t r = 0;
    for (int k = 63; k >= 0; --k) {
        idx_t bit   = 1ULL << k;
        idx_t cand  = r | bit;
        if (cand > 0 && cand <= x / cand)
            r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t t = (floor_sqrt_u64(8 * n - 7) - 1) / 2;          /* triangular index */
    idx_t triangular = t * (t + 1) / 2;
    idx_t i = n - triangular;
    idx_t j = ((t * t + 3 * t + 4) / 2) - n;

    seq_t pow_i = pow2(i);      /* 2^i */
    seq_t pow_j = pow2(j);      /* 2^j */

    return (pow_i - (seq_t)1) * pow_j + (seq_t)1;
}
