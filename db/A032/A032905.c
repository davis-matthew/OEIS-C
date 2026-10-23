/*
 * A032905  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer whose base‑8 digits satisfy d0 ≤ d1 ≥ d2
 *            ≤ d3 … (strict alternation starting with ≤)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#include <cstddef>
#include <cstdlib>
#else
#include <stdlib.h>
#include <stdint.h>
#endif

/* Check whether a value satisfies the alternating digit inequality in base‑8 */
static bool _is_valid(seq_t x)
{
    seq_t tmp = x;
    unsigned char digits[256];
    size_t len = 0;

    /* Extract digits in base 8 (least significant first) */
    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (unsigned char)(tmp & 7);   /* tmp % 8 */
            tmp >>= 3;                                 /* tmp / 8 */
        }
    }

    /* Verify inequalities */
    for (size_t i = 0; i + 1 < len; ++i) {
        if ((i & 1) == 0) {          /* even index: <= */
            if (digits[i] > digits[i + 1])
                return false;
        } else {                     /* odd index: >= */
            if (digits[i] < digits[i + 1])
                return false;
        }
    }
    return true;
}

#ifndef __cplusplus
/* C implementation: dynamic cache using malloc/realloc */
static seq_t* _cache = NULL;
static idx_t _cache_len = 0;
static idx_t _cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    while (_cache_len < n) {
        seq_t cand = (_cache_len == 0) ? (seq_t)1 : _cache[_cache_len - 1] + (seq_t)1;

        for (;;) {
            if (_is_valid(cand)) {
                if (_cache_len == _cache_cap) {
                    idx_t new_cap = (_cache_cap == 0) ? 16 : _cache_cap * 2;
                    seq_t* tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* allocation failed; abort as per specification */
                        abort();
                    }
                    _cache = tmp;
                    _cache_cap = new_cap;
                }
                _cache[_cache_len++] = cand;
                break;
            }
            ++cand;
        }
    }
    return _cache[n - 1];
}
#else
/* C++ implementation: use std::vector for automatic construction */
struct _CacheHolder {
    std::vector<seq_t> data;
    _CacheHolder() {}
};

static _CacheHolder _cache;

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_t(0);

    while (_cache.data.size() < n) {
        seq_t cand = (_cache.data.empty()) ? seq_t(1) : _cache.data.back() + seq_t(1);

        for (;;) {
            if (_is_valid(cand)) {
                _cache.data.push_back(cand);
                break;
            }
            ++cand;
        }
    }
    return _cache.data[n - 1];
}
#endif
