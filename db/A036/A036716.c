/*
 * A036716  a(n)=number of Gaussian integers z=a+bi satisfying
 *          n-1/2<|z|<=n+1/2, a>=0, 0<=b<=a.
 * https://oeis.org/A036716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Gaussian integers z = a+bi with n-½ < |z| ≤
 *            n+½, a ≥ 0, 0 ≤ b ≤ a. (OEIS A036716)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root (floor) for uint64_t */
static inline uint64_t floor_sqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Integer square root (ceil) for uint64_t */
static inline uint64_t ceil_sqrt_uint64(uint64_t x)
{
    uint64_t r = floor_sqrt_uint64(x);
    return (r * r < x) ? r + 1 : r;
}

/* Compute a single term a(n) */
static seq_t compute_value(idx_t n)
{
    if (n == 0) return (seq_t)1; /* |z| ≤ ½ only contains z = 0 */

    /* Bounds L4 = (2n-1)^2, U4 = (2n+1)^2  (all 64‑bit values) */
    uint64_t two_n = (uint64_t)2 * n;
    uint64_t L4 = (two_n - 1) * (two_n - 1);
    uint64_t U4 = (two_n + 1) * (two_n + 1);

    seq_t count = 0;

    for (idx_t a = 0; a <= n; ++a) {
        uint64_t a2      = a * a;
        uint64_t four_a2 = 4 * a2;

        if (four_a2 > U4) break;          /* 4a² exceeds upper bound */

        /* Upper bound on b² */
        uint64_t ub      = U4 - four_a2;   /* U4 >= four_a2 */
        uint64_t upper_b2= ub / 4;

        /* Lower bound on b² (strict inequality) */
        uint64_t lower_b2;
        if (L4 <= four_a2) {
            lower_b2 = 0;                  /* 4b² > negative → b² ≥ 0 */
        } else {
            lower_b2 = (L4 - four_a2) / 4 + 1;
        }

        if (lower_b2 > upper_b2) continue;

        uint64_t b_min = ceil_sqrt_uint64(lower_b2);
        uint64_t b_max = floor_sqrt_uint64(upper_b2);

        if (b_max > a) b_max = a;          /* enforce b ≤ a */
        if (b_min > a) continue;           /* no admissible b */

        if (b_max >= b_min)
            count += (seq_t)(b_max - b_min + 1);
    }

    return count;
}

/* External linkage: required function for the build system. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if ((idx_t)n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failure handled elsewhere */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = compute_value(i);
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
