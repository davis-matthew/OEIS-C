/*
 * A099887  XOR difference triangle of the powers of 3, read by rows.
 * https://oeis.org/A099887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = XOR of 3^(r-i) for all i ⊆ c, where n = r(r+1)/2 + c,
 *            0≤c≤r
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit unsigned values.  Uses binary search
   with 128‑bit arithmetic to avoid overflow. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (( (__uint128_t)mid * mid ) <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the row r and column c (0‑based) such that
       n = r(r+1)/2 + c, with 0 ≤ c ≤ r. */
    idx_t r, c;
    {
        idx_t t = (idx_t)(((__uint128_t)8 * n) + 1); /* 8*n + 1 fits in uint64_t for n up to 10⁹ */
        idx_t root = isqrt_u64(t);                  /* floor(sqrt(8*n+1)) */
        r = (root - 1) >> 1;                        /* (root-1)/2  */
        idx_t row_start = (idx_t)(((__uint128_t)r * (r + 1)) >> 1); /* r*(r+1)/2  */
        c = n - row_start;
    }

    /* Compute 3^r once. */
    seq_t pow3r = 1;
    for (idx_t e = 1; e <= r; ++e)
        pow3r *= (seq_t)3;

    /* Iterate over i from 0 to c.  pow_i = 3^i is kept track of
       incrementally.  Only indices i that are bitwise subsets of c
       contribute to the XOR.  The desired term is 3^(r-i) = pow3r / pow_i. */
    seq_t res = 0;
    seq_t pow_i = 1;
    for (idx_t i = 0; i <= c; ++i) {
        if ((i & ~c) == 0)          /* i ⊆ c under binary representation */
            res ^= (pow3r / pow_i);
        pow_i *= (seq_t)3;           /* advance to 3^(i+1) */
    }

    return res;
}
