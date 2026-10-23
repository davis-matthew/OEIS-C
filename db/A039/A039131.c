/*
 * A039131  Numbers whose base-10 representation has the same number of 2's
 *          and 6's.
 * https://oeis.org/A039131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   brute‑force search for numbers whose decimal representation
 *            contains the same number of 2’s and 6’s
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#else
#include <vector>
#include <cstddef>
#endif

/* Cached sequence values. */
#if defined(__cplusplus)
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
#endif
static idx_t next_candidate = 0;

/* Return true if x contains an equal number of decimal digits 2 and 6. */
static bool balanced(seq_t x) {
    int c2 = 0, c6 = 0;
    if (x == 0) return true;          /* 0 has 0 of each by definition */
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 2) c2++;
        else if (d == 6) c6++;
        x = x / 10;
    }
    return c2 == c6;
}

/* External function: a(n) returns the n-th term of OEIS A039131. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but safe to return 0 */

#if defined(__cplusplus)
    while (cache.size() < (size_t)n) {
        seq_t val = static_cast<seq_t>(next_candidate);
        if (balanced(val)) cache.push_back(val);
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
#else
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        if (balanced(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* memory exhaustion */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
    return cache[cache_len - 1];
#endif
}
