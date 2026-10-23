/*
 * A137201  "Ungodly" numbers: numbers that, in some base b > 6, contain the
 *          string 666 at least once in their expansion.
 * https://oeis.org/A137201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer that, in some base b > 6,
 *            contains the string "666" at least once in its base‑b expansion
 * Range:     u8 none, u16 n <= 1766, u32 n <= 2938, u64 n <= 2938,
 *            u128 n <= 2938, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#endif

/* Test whether a given integer n has, for some base b>6, the substring
 * "666" in its representation.  Only bases up to √n need be examined,
 * because larger bases produce representations with at most two digits. */
static bool has_666(uint64_t n) {
    for (uint64_t b = 7; b * b <= n; ++b) {
        uint64_t x = n;
        int d0 = -1, d1 = -1, d2 = -1;        /* previous digits */
        while (x > 0) {
            int d = (int)(x % b);
            if (d == 6 && d0 == 6 && d1 == 6) return true;
            d2 = d1;
            d1 = d0;
            d0 = d;
            x /= b;
        }
    }
    return false;
}

#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static uint64_t next_candidate = 342;   /* first term of the sequence */
#else
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_candidate = 342;
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* behaviour for n=0 is irrelevant */

#ifdef __cplusplus
    while (cache_vec.size() < n) {
        while (true) {
            if (has_666(next_candidate)) {
                cache_vec.push_back((seq_t)next_candidate);
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
    return cache_vec[n - 1];
#else
    while (cache_len < n) {
        while (true) {
            if (has_666(next_candidate)) {
                /* Grow cache if needed. */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
#endif
}
