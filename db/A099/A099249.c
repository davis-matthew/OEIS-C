/*
 * A099249  Number of numbers not greater than n such that length in binary
 *          representation and number of ones are coprime.
 * https://oeis.org/A099249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 401, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{ m : 1 ≤ m ≤ n, gcd(bitlen_2(m), popcount(m)) = 1 }
 * 
 * This implementation caches prefix sums so that repeated calls
 * with arbitrary n are efficient.  The cache grows lazily with `realloc`.
 * The routine uses GCC/Clang built‑ins for popcount and bit length.
 * It is valid C11 and C++17 and relies only on <stdlib.h>.
 */

#include <stdlib.h>
#include "../utils/gcd_uint64.h"

/* helper: popcount of a 64‑bit integer */
static inline uint64_t popcount_uint64(uint64_t v)
{
    return __builtin_popcountll(v);
}

/* helper: number of bits needed to represent v (v > 0) */
static inline uint64_t bitlen_uint64(uint64_t v)
{
    return 64 - __builtin_clzll(v);
}

/* helper: greatest common divisor of two 64‑bit integers */

/* cache of prefix sums a(k) for 0 ≤ k ≤ current_max */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of valid entries in cache (cache_len = last k + 1) */

/* public function */
seq_t a(idx_t n)
{
    /* initialise cache if unused */
    if (cache_len == 0) {
        cache_len = 1;                     /* index 0 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = 0;
    }

    /* extend cache to cover n */
    if ((idx_t)cache_len <= n) {
        idx_t new_len = n + 1;             /* need indices 0 … n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!new_cache) {
            /* fallback: no memory – return 0 (should never happen) */
            return 0;
        }
        cache = new_cache;
        /* compute new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            uint64_t len = bitlen_uint64(i);
            uint64_t ones = popcount_uint64(i);
            seq_t inc = (gcd_uint64(len, ones) == 1) ? 1 : 0;
            cache[i] = cache[i - 1] + inc;
        }
        cache_len = new_len;
    }

    return cache[n];
}
