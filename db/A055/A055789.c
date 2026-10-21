/*
 * A055789  a(n) = binomial(n, round(sqrt(n))).
 * https://oeis.org/A055789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n, round(sqrt(n)))
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 63, u64 n <= 177,
 *            u128 n <= 506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer floor of sqrt(n) using binary search and 128‑bit arithmetic. */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Euclidean algorithm for idx_t. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Binomial coefficient n choose k computed using only multiplication of
   small integers and integer division. Works for all seq_t sizes. */
static seq_t binomial_custom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return 1;

    idx_t *den = (idx_t *)malloc(k * sizeof(idx_t));
    if (!den) return 0;          /* fallback on allocation failure */

    for (idx_t i = 0; i < k; ++i) den[i] = i + 1;

    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        idx_t num = n - k + 1 + i;      /* numerator term */

        /* Reduce the numerator with all remaining denominators. */
        for (idx_t j = 0; j < k; ++j) {
            if (den[j] == 1) continue;
            idx_t g = gcd_idx(num, den[j]);
            if (g > 1) {
                num   /= g;
                den[j] /= g;
            }
        }
        res = res * (seq_t)num;
    }
    free(den);
    return res;
}

seq_t a(idx_t n)
{
    /* Determine the nearest integer to sqrt(n). */
    idx_t floor_sqrt = int_sqrt(n);
    idx_t ceil_sqrt  = floor_sqrt + 1;

    __int128 dist_floor = (__int128)n - (__int128)floor_sqrt * floor_sqrt;
    __int128 dist_ceil  = (__int128)ceil_sqrt * ceil_sqrt - (__int128)n;

    idx_t k = floor_sqrt;
    if (dist_floor > dist_ceil) k = ceil_sqrt;   /* round up if closer */

    return binomial_custom(n, k);
}
