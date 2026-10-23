/*
 * A028601  Expansion of (theta_3(z)*theta_3(9z) + theta_2(z)*theta_2(9z)).
 * https://oeis.org/A028601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of integer solutions (x,y) satisfying:
 *   if n even:    x² + 9 y² = n/2
 *   if n odd :    (2x+1)² + 9 (2y+1)² = 2n
 * This implements the coefficient of q^(n/2) in θ₃(z)·θ₃(9z) + θ₂(z)·θ₂(9z).
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer (floor of sqrt) */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t lo = 1, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* even n:  x² + 9y² = n/2 */
        idx_t m   = n >> 1;            /* n/2 using shift */
        seq_t  cnt = 0;

        /* y = 0 case */
        {
            idx_t rem = m;
            idx_t s   = isqrt_u64(rem);
            if (s * s == rem) {
                cnt += (s == 0) ? 1 : 2;   /* (±x,0) or (0,0) */
            }
        }

        /* y > 0 cases: two signs of y */
        for (idx_t y = 1; 9 * y * y <= m; ++y) {
            idx_t rem = m - 9 * y * y;
            idx_t s   = isqrt_u64(rem);
            if (s * s == rem) {
                cnt += (s == 0) ? 2 : 4;   /* (±x,±y) */
            }
        }

        return cnt;
    }

    /* odd n: (2x+1)² + 9(2y+1)² = 2n */
    idx_t target = n << 1;            /* 2*n */
    seq_t cnt = 0;

    for (idx_t v = 1; 9 * v * v <= target; v += 2) {  /* v = 2y+1 odd */
        idx_t rem = target - 9 * v * v;
        idx_t u   = isqrt_u64(rem);
        if (u * u == rem && (u & 1)) {              /* u odd */
            cnt += 4;                               /* (±u,±v) */
        }
    }

    return cnt;
}
