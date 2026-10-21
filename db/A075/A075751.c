/*
 * A075751  Numbers n such that 210*n -/+ 13 are primes.
 * https://oeis.org/A075751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that 210*n-13 and 210*n+13 are primes.  */
/* Returns a(n), the nth such n, with a(1)=1.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> _cache;
static idx_t _next_cand = 1;   /* next n to test */
#else
static seq_t * _cache = NULL;
static size_t _cache_len = 0;
static size_t _cache_cap = 0;
static idx_t _next_cand = 1;   /* next n to test */
#endif

/* Helper: true iff both 210*n-13 and 210*n+13 are prime. */
static bool _good_n(idx_t n_val)
{
    uint64_t n = (uint64_t)n_val;
    uint64_t a = 210ULL * n - 13ULL;
    uint64_t b = 210ULL * n + 13ULL;
    return is_prime(a) && is_prime(b);
}

seq_t a(idx_t n)
{
    /* Ensure the cache has at least n terms. */
#ifdef __cplusplus
    while (_cache.size() < (size_t)n) {
        /* Find the next qualifying n. */
        while (!_good_n(_next_cand)) ++_next_cand;
        _cache.push_back((seq_t)_next_cand);
        ++_next_cand;
    }
    return _cache[n - 1];
#else
    /* Lazy initialization of the cache. */
    if (_cache == NULL) {
        _cache_cap = 4;
        _cache = (seq_t*)malloc(_cache_cap * sizeof(seq_t));
        _cache_len = 0;
    }
    while (_cache_len < n) {
        /* Skip non‑qualifying candidates. */
        while (!_good_n(_next_cand)) ++_next_cand;
        /* Grow the array if necessary. */
        if (_cache_len == _cache_cap) {
            _cache_cap *= 2;
            _cache = (seq_t*)realloc(_cache, _cache_cap * sizeof(seq_t));
        }
        _cache[_cache_len++] = (seq_t)_next_cand;
        ++_next_cand;
    }
    return _cache[n - 1];
#endif
}
