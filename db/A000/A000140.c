/*
 * A000140  Kendall-Mann numbers: the most common number of inversions in a
 *          permutation on n letters is floor(n*(n-1)/4); a(n) is the number
 *          of permutations with this many inversions.
 * https://oeis.org/A000140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = coefficient of x^floor(n*(n-1)/4) in Product_{k=1..n} (1
 *            + x + ... + x^(k-1)). Computed by building the product step by
 *            step, keeping only coefficients up to x^floor(n*(n-1)/4). Each
 *            step divides by (1-x) (a prefix sum) and multiplies by (1-x^k).
 *            a(k) is recorded for every k <= n along the...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_series.h"

static seq_t *a140_cache = 0;
static idx_t a140_cached_n = 0;

static void a140_build(idx_t N)
{
    idx_t M = N * (N - 1) / 4;
    idx_t i, k;
    seq_t *c = alloc_series((size_t)(M + 1));
    seq_t *res = alloc_series((size_t)(N + 1));
    for (i = 0; i <= M; i++) c[i] = 0;
    for (i = 0; i <= N; i++) res[i] = 0;
    c[0] = 1;
    res[1] = 1;
    for (k = 2; k <= N; k++) {
        idx_t deg = k * (k - 1) / 2;
        idx_t top = deg < M ? deg : M;
        idx_t mid = k * (k - 1) / 4;
        /* divide by (1 - x): prefix sums */
        for (i = 1; i <= top; i++) c[i] += c[i - 1];
        /* multiply by (1 - x^k) */
        for (i = top; i >= k; i--) c[i] -= c[i - k];
        res[k] = c[mid];
    }
    a140_cache = res;
    a140_cached_n = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 1;
    if (a140_cache == 0 || n > a140_cached_n) a140_build(n);
    return a140_cache[n];
}
