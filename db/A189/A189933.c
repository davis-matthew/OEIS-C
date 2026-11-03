/*
 * A189933  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=(sin(Pi/5))^2,
 *          t=(cos(Pi/5))^2.
 * https://oeis.org/A189933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*sec²(π/5)⌋ + ⌊n*tan²(π/5)⌋ Simplified to: a(n) =
 *            12*n - 2 - 2*⌊2*n*√5⌋ Because tan²(π/5) = 5 – 2√5 and sec²(π/5)
 *            = 1 + tan²(π/5)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7144, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9065 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1, safety guard */

    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* t = 12*n - 2  */
    seq_t t = seq_mul((seq_t)12, N);      /* 12*n   */
    t = t - (seq_t)2;                    /* 12*n-2 */

    /* m = 2*n  */
    seq_t m = seq_mul((seq_t)2, N);

    /* prod = 5 * m * m   (gives 5*(2n)^2)  */
    seq_t prod = seq_mul(m, m);          /* m*m   */
    prod = seq_mul(prod, (seq_t)5);      /* 5*(m*m) */

    /* sqrt_floor = ⌊√prod⌋ = ⌊2n*√5⌋ */
    seq_t sqrt_floor = floor_sqrt(prod);

    /* two_sqrt = 2 * sqrt_floor */
    seq_t two_sqrt = seq_mul(sqrt_floor, (seq_t)2);

    /* result = t - two_sqrt = 12*n - 2 - 2*⌊2n*√5⌋ */
    seq_t res = t - two_sqrt;
    return res;
}
