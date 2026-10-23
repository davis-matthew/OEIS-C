/*
 * A147771  a(n) = round(n^(n/2)).
 * https://oeis.org/A147771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n^(n/2)) (OEIS A147771)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Utility headers that may be available */
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

static inline __uint128_t mul128(__uint128_t a, __uint128_t b)
{
    return a * b;   /* compiler will use 128‑bit arithmetic */
}

static __uint128_t pow_uint128(__uint128_t base, idx_t exp)
{
    __uint128_t res = 1;
    while (exp > 0) {
        if (exp & 1) res = mul128(res, base);
        base = mul128(base, base);
        exp >>= 1;
    }
    return res;
}

static uint64_t floor_sqrt_uint128(__uint128_t x)
{
    uint64_t lo = 0, hi = UINT64_MAX;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t mid2 = mul128((__uint128_t)mid, (__uint128_t)mid);
        if (mid2 == x) return mid;
        if (mid2 < x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;   /* hi is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Build-time split: use 128‑bit arithmetic for 64‑bit seq_t,
       otherwise fall back to the generic big‑int utilities. */
    if (sizeof(seq_t) <= 8) {
        /* Small integer implementation ------------------------------------ */
        if ((n & 1ULL) == 0) {                 /* n even */
            idx_t e = n >> 1;                  /* e = n/2 */
            return (seq_t)pow_uint128((__uint128_t)n, e);
        }

        /* n odd ----------------------------------------------------------- */
        idx_t e = (n - 1) >> 1;                /* e = (n-1)/2 */
        __uint128_t t = pow_uint128((__uint128_t)n, e);  /* t = n^((n-1)/2) */

        /* Compute n^n = t^2 * n , all in 128 bits (safe for n where a(n)
           fits in 64 bits). */
        __uint128_t nN = mul128(t * t, (__uint128_t)n);

        /* Integer square root of n^n */
        uint64_t s = floor_sqrt_uint128(nN);

        /* Rounding: compare distances to nearest integer */
        __uint128_t s_sq   = mul128((__uint128_t)s, (__uint128_t)s);
        __uint128_t s1     = (__uint128_t)(s + 1);
        __uint128_t s1_sq  = mul128(s1, s1);

        __uint128_t diff_low  = nN - s_sq;
        __uint128_t diff_high = s1_sq - nN;

        return (diff_high < diff_low) ? (seq_t)(s + 1) : (seq_t)s;
    } else {
        /* Generic big‑integer implementation -------------------------------- */
        if ((n & 1) == 0) {                                           /* even */
            return big_pow((seq_t)n, (seq_t)(n / 2));
        }

        /* odd */
        seq_t X = big_pow((seq_t)n, (seq_t)n);                          /* n^n   */
        seq_t s = floor_sqrt(X);                                        /* floor(sqrt(n^n)) */
        seq_t s1 = s + (seq_t)1;
        seq_t diff_low  = X - seq_mul(s, s);
        seq_t diff_high = seq_mul(s1, s1) - X;
        return (diff_high < diff_low) ? s1 : s;
    }
}
