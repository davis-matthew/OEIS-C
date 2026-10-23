/*
 * A043599  Numbers whose base-4 representation has exactly 8 runs.
 * https://oeis.org/A043599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers whose base‑4 representation has exactly 8 runs (OEIS A043599). */

#include <stdlib.h>
#include <stdbool.h>

/* Count the number of runs in the base‑4 representation of v.
   A run is a maximal block of equal digits. */
static int count_runs(seq_t v) {
    int runs = 0;
    bool first = true;
    int prev = -1;
    while (v > 0) {
        int digit = (int)(v % 4);
        if (first) {
            runs = 1;
            prev = digit;
            first = false;
        } else if (digit != prev) {
            ++runs;
            prev = digit;
        }
        v /= 4;
    }
    return runs;
}

/* Cache of values found so far and the last checked sequence number. */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
static seq_t last_checked = 0;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;
static seq_t last_checked = 0;
#endif

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined value, but harmless */

    size_t target = (size_t)n;

#ifdef __cplusplus
    /* Grow the cache until it contains at least target elements. */
    while (cache.size() < target) {
        ++last_checked;
        if (count_runs(last_checked) == 8)
            cache.push_back(last_checked);
    }
    return cache[target - 1];
#else
    while (cache_size < target) {
        ++last_checked;
        if (count_runs(last_checked) == 8) {
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();     /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }
    return cache[target - 1];
#endif
}
