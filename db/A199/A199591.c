/*
 * A199591  Generalized Fermat numbers: 5^(2^n) + 1, n >= 0.
 * https://oeis.org/A199591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=6; for n>=1: a(n) = (a(n-1)-1)^2 + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies the types seq_t and idx_t; only the implementation
 * of the function a(idx_t) is required.  We cache previously computed
 * terms so that calls in any order are O(1) amortised.  The implementation
 * works for both C11 and C++17, including arbitrary‑precision seq_t. */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Runtime cache routines ------------------------------------------------ */
/* --------------------------------------------------------------------- */

#ifdef __cplusplus
/* C++ implementation: use std::vector. */
static std::vector<seq_t> _cache;

static void _ensure_cache_cpp(idx_t n) {
    if (_cache.size() > n) return;
    size_t old = _cache.size();
    _cache.resize((size_t)n + 1);
    for (idx_t i = old; i <= n; ++i) {
        if (i == 0) {
            _cache[i] = 6;               /* a(0) */
        } else {
            seq_t t = _cache[i - 1] - 1;
            t = t * t;
            _cache[i] = t + 1;
        }
    }
}

seq_t a(idx_t n) {
    _ensure_cache_cpp(n);
    return _cache[(size_t)n];
}

#else
/* C implementation: use a raw dynamic array. */
static seq_t * _cache = NULL;
static idx_t _cache_len = 0;        /* number of stored elements */

static void _ensure_cache_c(idx_t n) {
    if (_cache_len > n) return;
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(_cache, (size_t)new_len * sizeof(seq_t));
    _cache = tmp;                          /* realloc never fails here */
    for (idx_t i = _cache_len; i <= n; ++i) {
        if (i == 0) {
            _cache[i] = 6;                   /* a(0) */
        } else {
            seq_t t = _cache[i - 1] - 1;
            t = t * t;
            _cache[i] = t + 1;
        }
    }
    _cache_len = new_len;
}

seq_t a(idx_t n) {
    _ensure_cache_c(n);
    return _cache[(size_t)n];
}
#endif
