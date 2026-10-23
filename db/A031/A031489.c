/*
 * A031489  Numbers whose base-8 representation has 2 more 0's than 7's.
 * https://oeis.org/A031489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑8 representation has 2 more 0's than 7's. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#if defined(__cplusplus)
# include <vector>
#endif

/* Test whether a number satisfies the condition. */
static bool is_special(seq_t x) {
    int count0 = 0, count7 = 0;
    do {
        int d = (int)(x % 8);
        if (d == 0) count0++;
        else if (d == 7) count7++;
        x /= 8;
    } while (x != 0);
    return count0 == count7 + 2;
}

/* Dynamic storage for generated terms. */
#if defined(__cplusplus)
static std::vector<seq_t> cache;
static seq_t cur = 1;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_count = 0;
static seq_t cur = 1;
#endif

/* External linkage required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* Undefined for n < 1; not used. */

    /* Generate terms until we have at least n of them. */
    while (true) {
#if defined(__cplusplus)
        if (cache.size() >= (size_t)n) break;
        /* Search for the next qualifying number. */
        while (true) {
            if (is_special(cur)) break;
            cur += 1;
        }
        cache.push_back(cur);
        cur += 1;
#else
        if (cache_count >= (size_t)n) break;
        /* Search for the next qualifying number. */
        while (true) {
            if (is_special(cur)) break;
            cur += 1;
        }
        /* Ensure enough space in the array. */
        if (cache_count == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_count++] = cur;
        cur += 1;
#endif
    }

#if defined(__cplusplus)
    return cache.at((size_t)n - 1);
#else
    return cache[(size_t)n - 1];
#endif
}
