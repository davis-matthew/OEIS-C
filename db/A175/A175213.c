/*
 * A175213  a(n)= a(r)+a(s) ; (r+s)<=n ; r=(floor(sqrt(n-1)))^2 ;
 *          s=n-(floor(sqrt(n)))^2.
 * https://oeis.org/A175213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(r)+a(s) * where r = (floor(sqrt(n-1)))^2 * s = n -
 *            (floor(sqrt(n)))^2 * and a(0) = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* integer square root: floor(sqrt(x)) for unsigned 64‑bit */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t left = 1, right = x >> 1, mid, ans = 1;
    while (left <= right) {
        mid = left + (right - left) / 2;
        if (mid <= x / mid) {         /* mid^2 <= x */
            ans = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of computed elements */

    /* initialise cache for a(0) */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* abort if memory fails */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n == 0) return cache[0];

    /* extend the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* abort if memory fails */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) continue;               /* already set */
        idx_t idx = (idx_t)i;
        idx_t sqrt_n  = isqrt_u64(idx);
        idx_t sqrt_n1 = isqrt_u64(idx - 1);
        idx_t r = sqrt_n1 * sqrt_n1;
        idx_t s = idx - sqrt_n * sqrt_n;
        cache[i] = cache[r] + cache[s];
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
