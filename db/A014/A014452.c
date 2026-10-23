/*
 * A014452  Theta series of quadratic form with Gram matrix [ 1, 0, 0; 0, 2,
 *          1; 0, 1, 2 ].
 * https://oeis.org/A014452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 437, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Theta series of quadratic form with Gram matrix 
 * [ 1, 0, 0; 0, 2, 1; 0, 1, 2 ].
 * a(n) = # {(x,y,z)∈ℤ³ : x² + 2(y²+yz+z²) = n}
 * 
 * Implementation uses an explicit enumeration over x, y, z.
 * Complexity O(√n·√(n/2)), fast enough for n≤10⁴.
 */

#include <stdint.h>

/* integer square root for an unsigned 64‑bit integer */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* count representations of m by y² + yz + z² */
static seq_t count_pairs(idx_t m)
{
    seq_t cnt = 0;
    idx_t max_y = isqrt_idx(2 * m);          /* bound for |y| */
    for (idx_t abs_y = 0; abs_y <= max_y; ++abs_y) {
        idx_t y_sq = abs_y * abs_y;
        idx_t d = 4 * m - 3 * y_sq;
        if (d > 4 * m) /* overflow guard, though not needed */ continue;
        if (d > 0 && d > 4 * m) continue;    /* cannot happen */
        if (d > (idx_t)(-1)) continue;       /* overflow guard */
        idx_t sqrt_d = isqrt_idx(d);
        if (sqrt_d * sqrt_d != d) continue;  /* not a perfect square */

        seq_t zcnt = (sqrt_d == 0) ? 1 : 2;   /* 1 or 2 solutions for z */

        if (abs_y == 0) {
            cnt += zcnt;
        } else {
            cnt += zcnt * 2;   /* y and -y give distinct pairs */
        }
    }
    return cnt;
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t sqrt_n = isqrt_idx(n);

    for (idx_t abs_x = 0; abs_x <= sqrt_n; ++abs_x) {
        idx_t x_sq = abs_x * abs_x;
        idx_t rem = n - x_sq;
        if ((rem & 1ULL) != 0ULL) continue;       /* odd remainder cannot be 2*(...) */

        idx_t m = rem >> 1;                       /* m = (n - x²)/2 */
        seq_t pair_cnt = count_pairs(m);

        if (abs_x == 0) {
            result += pair_cnt;                   /* x = 0 gives one sign */
        } else {
            result += pair_cnt * 2;               /* x and -x */
        }
    }
    return result;
}
