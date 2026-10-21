/*
 * A218381  Numbers k = i*j such that i+j is a square.
 * https://oeis.org/A218381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that there exist positive integers i,j with k =
 *            i*j and i + j is a perfect square
 * Range:     u8 n <= 51, u16 n <= 4216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple incremental sieve: it tests each
   integer k ≥ 3 and keeps the ones that satisfy the condition in a
   cache.  A small integer square‑root routine serves as an
   independent stand‑in for any external isqrt helper. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- integer square root for 64‑bit values ---------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* hi > sqrt(UINT64_MAX) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)n) return mid;
        if (sq < (unsigned __int128)n) lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* ---------- check whether a given k has i + j = square ---------- */
static bool good_k(idx_t k)
{
    idx_t lim = isqrt_u64(k);
    for (idx_t i = 1; i <= lim; ++i) {
        if (k % i != 0) continue;
        idx_t j   = k / i;
        idx_t s   = i + j;
        idx_t r   = isqrt_u64(s);
        if ((unsigned __int128)r * r == (unsigned __int128)s) return true;
    }
    return false;
}

/* ---------- cache for computed values ---------- */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 3;      /* smallest k that can appear */

/* ensure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    while (cache_cap < need) cache_cap = cache_cap * 2 + 1;
    seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
}

/* ---------- public function required by the harness ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for n == 0 */

    /* lazy cache initialisation */
    if (seq_cache == NULL) {
        cache_cap = 256;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < (size_t)n) {
        if (good_k(next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[(size_t)n - 1];
}
