/*
 * A191452  Dispersion of (4,8,12,16,...), by antidiagonals.
 * https://oeis.org/A191452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(i) * 4^(j-1) where the nth antidiagonal entry lies in
 *            row i and column j of the array D(i,j) = t(i) * 4^(j-1), with
 *            t(i) = i + floor((i-1)/3). The row and column indices are
 *            determined from n by the antidiagonal ordering
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 136, u64 n <= 528,
 *            u128 n <= 2080, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* integer square root for 128‑bit values, result fits in a 64‑bit idx_t */
static inline idx_t u128_isqrt(__uint128_t v)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* 2^32 > sqrt(2^128) */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t prod = (__uint128_t)mid * mid;
        if (prod <= v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* compute the row index s (i+j) for the given n (1‑based) */
static inline idx_t find_row(idx_t n)
{
    if (n < 1) return 0;          /* undefined, just return 0 */
    __uint128_t val = (__uint128_t)8 * n + 1;
    idx_t sqrt_val = u128_isqrt(val);
    idx_t s = (1 + sqrt_val) / 2;  /* floor of the quadratic solution */

    /* ensure (s-1)*s/2 >= n; adjust if necessary */
    while (((__uint128_t)(s - 1) * s) / 2 < n) s++;
    return s;
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard */

    /* 1. Determine the antidiagonal (row sum) */
    idx_t s = find_row(n);

    /* 2. Determine row i and column j */
    idx_t prev_tri = ((__uint128_t)(s - 2) * (s - 1)) / 2; /* elements before row s */
    idx_t offset  = n - prev_tri - 1;                    /* 0‑based offset in this row */
    idx_t i = offset + 1;                                /* row index (1‑based) */
    idx_t j = s - i;                                     /* column index (1‑based) */

    /* 3. Compute t(i) = i + floor((i-1)/3) */
    seq_t ti = (seq_t)i + (seq_t)((i - 1) / 3);

    /* 4. Compute 4^(j-1) = 2^(2*(j-1)) using pow2 utility */
    seq_t pow4 = pow2(2 * (j - 1));

    /* 5. Return the value */
    return ti * pow4;
}
