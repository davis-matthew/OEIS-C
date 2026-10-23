/*
 * A044906  Numbers whose base-5 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 run lengths alternate: odd, even, odd, … */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Test whether a number has alternating odd/even run lengths in base 5. */
static bool _is_good(seq_t x)
{
    if (x == 0) return false;          /* numbers start at 1 */

    uint8_t digits[1024];              /* enough for all needed values */
    size_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 5);
        tmp /= 5;
    }
    if (len == 0) return false;        /* should not happen */

    size_t idx = len - 1;
    seq_t run_len = 1;
    bool expected_odd = true;          /* first run must be odd */
    uint8_t prev = digits[idx];

    for (idx--; idx < len; --idx) {
        uint8_t d = digits[idx];
        if (d == prev) {
            run_len++;
        } else {
            if ((run_len & 1) != expected_odd) return false;
            expected_odd = !expected_odd;
            run_len = 1;
            prev = d;
        }
    }
    /* final run check */
    if ((run_len & 1) != expected_odd) return false;
    return true;
}

#ifdef __cplusplus
/* C++ implementation using std::vector */
static std::vector<seq_t> cache;

static void _ensure_cache(idx_t n)
{
    if (cache.size() >= n) return;

    seq_t cur = (cache.empty() ? (seq_t)1 : cache.back() + 1);
    while (cache.size() < n) {
        if (_is_good(cur))
            cache.push_back(cur);
        cur += (seq_t)1;
    }
}
#else
/* C implementation using a dynamic array */
static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;
static idx_t  cache_cap  = 0;

static void _ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    seq_t cur = (cache_len > 0 ? cache[cache_len-1] + (seq_t)1 : (seq_t)1);
    while (cache_len < n) {
        if (_is_good(cur)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap ? cache_cap * 2 : 8);
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = cur;
        }
        cur += (seq_t)1;
    }
}
#endif

/* Public sequence function */
seq_t a(idx_t n)
{
    _ensure_cache(n);
#ifdef __cplusplus
    return cache[n-1];
#else
    return cache[n-1];
#endif
}
