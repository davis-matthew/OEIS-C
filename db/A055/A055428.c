/*
 * A055428  Number of points in Z^n of norm <= 4.
 * https://oeis.org/A055428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer vectors (z1,…,zn) with * z1²+…+zn² ≤
 *            16. The generating function per * coordinate is
 *            1+2x+2x⁴+2x⁹+2x¹⁶, so a(n) is the sum * of coefficients for
 *            x⁰,…,x¹⁶ in its nth power
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 61, u128 n <= 877,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *alloc_cache(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

static void free_cache(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Per‑coordinate generating function coefficients
     * G(x) = 1 + 2x + 2x⁴ + 2x⁹ + 2x¹⁶. */
    static const seq_t g[17] = {1, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2};

    /* Cache of a(k) for 0 ≤ k ≤ last_n. */
    static seq_t *cache = NULL;      /* array of size cache_cap */
    static size_t  cache_cap = 0;    /* number of elements allocated */
    static idx_t   last_n    = 0;    /* highest n for which cache is valid */

    /* Temporary holds the counts of exact squared norms for current n. */
    static seq_t dp[17];
    seq_t newdp[17];

    /* Initialise for n = 0 if this is the first call. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = alloc_cache(cache_cap);
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        dp[0] = (seq_t)1;
        for (size_t i = 1; i < 17; ++i) dp[i] = (seq_t)0;
        last_n = 0;
    }

    /* If the requested n is already cached, return it. */
    if (n <= last_n) {
        return cache[(size_t)n];
    }

    /* Ensure the cache has room for up to n. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *new_cache = alloc_cache(new_cap);
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];
        }
        free_cache(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Iterate dimensions from last_n+1 up to n. */
    for (idx_t cur = last_n + 1; cur <= n; ++cur) {
        /* Convolution dp * g */
        for (int s = 0; s <= 16; ++s) {
            newdp[s] = (seq_t)0;
            for (int t = 0; t <= 16; ++t) {
                if (t > s) break;
                if (g[t] == (seq_t)0) continue;
                newdp[s] += dp[s - t] * g[t];
            }
        }
        /* Copy back to dp */
        for (int s = 0; s <= 16; ++s) {
            dp[s] = newdp[s];
        }

        /* Compute a(cur) = sum_{s=0}^{16} dp[s] */
        seq_t total = (seq_t)0;
        for (int s = 0; s <= 16; ++s) {
            total += dp[s];
        }
        cache[(size_t)cur] = total;
    }

    last_n = n;
    return cache[(size_t)n];
}
