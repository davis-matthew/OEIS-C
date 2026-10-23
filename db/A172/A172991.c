/*
 * A172991  Triangle of binomial sums read by rows: T(n,k) = sum(C(2n-2k-i,i)
 *          * C(2k-i,i), i=0..min(k,n-k)).
 * https://oeis.org/A172991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = read‑by‑row value of the triangle T(n,k) =
 *            Σ_{i=0}^{min(k,n-k)} C(2n-2k-i,i)·C(2k-i,i)
 * Range:     u8 n <= 39, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;           /* flattened triangle */
static size_t cache_len = 0;          /* number of elements stored          */
static size_t cache_cap = 0;          /* capacity of cache array             */

/* Compute a single triangle entry T(n,k) */
static seq_t compute_T(idx_t n, idx_t k)
{
    idx_t max_i = (k <= n - k) ? k : n - k;
    seq_t sum = 0;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t a = binom((idx_t)(2 * n - 2 * k - i), (idx_t)i);
        seq_t b = binom((idx_t)(2 * k - i), (idx_t)i);
        sum += a * b;
    }
    return sum;
}

/* Extend the cache to contain at least indices up to target */
static void ensure_cache_upto(idx_t target)
{
    if (target < cache_len)
        return;

    /* row containing the target index */
    idx_t target_row = (idx_t)((sqrt(8.0 * (double)target + 1.0) - 1.0) / 2.0);

    /* new total number of elements after extending */
    idx_t new_len = (target_row + 1) * (target_row + 2) / 2;

    /* grow the underlying array if needed */
    if (new_len > cache_cap) {
        size_t cap = cache_cap > 0 ? cache_cap : 8;
        while (cap < new_len)
            cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        cache_cap = cap;
    }

    /* first row that has not yet been computed */
    idx_t start_row = (idx_t)((sqrt(8.0 * (double)cache_len + 1.0) - 1.0) / 2.0);

    for (idx_t r = start_row; r <= target_row; ++r) {
        for (idx_t k = 0; k <= r; ++k) {
            cache[(r * (r + 1)) / 2 + k] = compute_T(r, k);
        }
    }

    cache_len = new_len;
}

/* External sequence function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache_upto(n);
    return cache[n];
}
