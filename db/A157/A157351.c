/*
 * A157351  Row sums of triangle T(j,k) = (j^k) mod (j*k) for 1 <= k <= j (see
 *          A096133).
 * https://oeis.org/A157351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} (n^k mod (n*k))
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* static cache for memoisation of computed values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure that values up to n are computed and stored in 'cache' */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    /* grow cache array to at least n */
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();

    cache = new_cache;

    /* compute new rows */
    for (idx_t j = cache_cap + 1; j <= n; ++j) {
        seq_t sum = 0;
        seq_t jseq = (seq_t)j;
        for (idx_t k = 1; k <= j; ++k) {
            /* use 64‑bit intermediate to avoid overflow */
            uint64_t prod = (uint64_t)j * (uint64_t)k;
            seq_t mod = (seq_t)prod;
            seq_t term = mod_pow(jseq, k, mod);
            sum += term;
        }
        cache[j] = sum;
    }

    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* safe guard for offset */
    ensure_cache(n);
    return cache[n];
}
