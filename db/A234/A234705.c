/*
 * A234705  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-1) + 84*a(n-2) - 420*a(n-3) - 2336*a(n-4)
 *            + 11680*a(n-5) + 21504*a(n-6) - 107520*a(n-7).
 * Initial terms (1‑based):
 *   a(1)=322, a(2)=1730, a(3)=8738, a(4)=47306,
 *   a(5)=239890, a(6)=1308890, a(7)=6664130.
 */
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* cache the computed values (0‑based array) */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    /* return 0 for indices before the offset */
    if (n < 1u) return 0;

    /* initialise cache with the first 7 terms on first call */
    static const seq_t base_vals[7] = {322ULL, 1730ULL, 8738ULL,
                                       47306ULL, 239890ULL, 1308890ULL,
                                       6664130ULL};

    if (!cache) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        assert(cache);
        for (size_t i = 0; i < 7; ++i) cache[i] = base_vals[i];
        cached_len = 7;
    }

    /* already computed? */
    if ((size_t)n <= cached_len) return cache[(size_t)n - 1];

    /* extend cache to required size */
    size_t old_len = cached_len;
    cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    assert(cache);
    cached_len = (size_t)n;

    /* compute terms from old_len up to n-1 (0‑based indices) */
    for (size_t i = old_len; i < cached_len; ++i) {
        /* i corresponds to a(i+1) in 1‑based indexing */
        seq_t pos = 5 * cache[i - 1]
                 + 84 * cache[i - 2]
                 + 11680 * cache[i - 5]
                 + 21504 * cache[i - 6];

        seq_t neg = 420 * cache[i - 3]
                 + 2336 * cache[i - 4]
                 + 107520 * cache[i - 7];

        cache[i] = pos - neg;     /* pos > neg for all n ≥ 8 */
    }

    return cache[(size_t)n - 1];
}
