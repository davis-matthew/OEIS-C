/*
 * A140944  Triangle T(n,k) read by rows, the k-th term of the n-th
 *          differences of the Jacobsthal sequence A001045.
 * https://oeis.org/A140944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,k) r = floor( (-1 + sqrt(1+8n))/2 ) k = n - r(r+1)/2
 *            T(r,k)= (2^k - 2^r*(-1)^(r+k))/3, for r ≥ k ≥ 0 (see OEIS
 *            A140944)
 * Range:     i8 n <= 27, i16 n <= 149, i32 n <= 557, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* integer square root of a 64‑bit unsigned integer */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* sqrt(2^64-1) < 2^32, so 31..0 inclusive is sufficient */
    for (int i = 31; i >= 0; --i)
    {
        idx_t bit = ((idx_t)1 << i);
        idx_t cand = r + bit;
        __uint128_t sq = (__uint128_t)cand * cand;
        if (sq <= x)
            r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* locate the row r that contains index n */
    idx_t t = 8 * n + 1;            /* t = 8n+1 */
    idx_t s = isqrt_u64(t);        /* s = floor( sqrt(t) ) */
    idx_t r = (s - 1) / 2;          /* initial guess for row */

    /* correct r if needed (safe for all n) */
    while ((__uint128_t)r * (r + 1) / 2 > n)
        --r;
    while ((__uint128_t)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* position within the row */
    idx_t k = n - ((idx_t)r * (r + 1) / 2);

    /* powers of two (using the provided utility) */
    seq_t powk = pow2(k);
    seq_t powr = pow2(r);

    /* sign = (-1)^(r+k) */
    seq_t sign = ((r + k) & 1) ? (seq_t)-1 : (seq_t)1;

    /* numerator: 2^k - 2^r * sign */
    seq_t numer = powk;
    if (sign == 1)
        numer -= powr;
    else
        numer += powr;

    /* division by 3 (the result is always divisible by 3) */
    seq_t result = numer / (seq_t)3;

    return result;
}
