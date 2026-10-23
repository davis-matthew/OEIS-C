/*
 * A027555  Triangle of binomial coefficients C(-n,k).
 * https://oeis.org/A027555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice, easy
 * Formula:   a(n) = (-1)^k * binomial(r + k - 1 , k) where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2
 * Range:     i8 n <= 25, i16 n <= 63, i32 n <= 187, i64 n <= 628,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 64‑bit x */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;   /* hi^2 fits in 128 bit */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Multiplicative binomial coefficient: binomial(a, k) for a>=k.
   Uses 128‑bit intermediate when seq_t fits in 64 bits to avoid overflow.
   The result is truncated to seq_t; for arbitrary‑precision seq_t the
   underlying type handles larger values. */
static seq_t binom(uint64_t a, uint64_t k)
{
    if (k > a) return 0;
    if (k > a - k) k = a - k;
    if (sizeof(seq_t) <= 8) {
        __int128 r = 1;
        for (uint64_t i = 1; i <= k; ++i) {
            __int128 num = (__int128)(a - k + i);
            r = r * num / i;
        }
        return (seq_t)r;
    } else {
        seq_t r = 1;
        for (uint64_t i = 1; i <= k; ++i) {
            seq_t num = (seq_t)(a - k + i);
            r = (r * num) / i;
        }
        return r;
    }
}

seq_t a(idx_t n)
{
    /* Convert index to unsigned 64‑bit for calculations */
    uint64_t idx = (uint64_t)n;

    /* Find row r: largest r with r*(r+1)/2 <= idx.  */
    uint64_t tmp = 8 * idx + 1;               /* 8*idx+1 fits in 65 bits */
    uint64_t s   = isqrt_u64(tmp);           /* floor(sqrt(8*idx+1)) */
    uint64_t r   = (s - 1) >> 1;              /* floor((s-1)/2) */
    /* Adjust in case of rounding */
    if ((r + 1) * (r + 2) / 2 <= idx) ++r;

    /* Column index k */
    uint64_t start_of_row = r * (r + 1) / 2;
    uint64_t k            = idx - start_of_row;

    /* Compute value */
    if (k == 0) {
        return 1;                            /* first column is always 1 */
    } else {
        uint64_t a_val = r + k - 1;          /* a = r + k - 1 for binomial */
        seq_t val = binom(a_val, k);
        if (k & 1)                            /* apply (-1)^k */
            val = -val;
        return val;
    }
}
