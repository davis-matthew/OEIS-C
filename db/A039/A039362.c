/*
 * A039362  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 6's.
 * https://oeis.org/A039362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039362: numbers whose base‑9 representation has an equal positive number of 0’s and 6’s. Search-based implementation. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Return true iff val has the same positive number of 0 and 6 in base‑9. */
static bool a039362_valid(seq_t val)
{
    unsigned cnt0 = 0, cnt6 = 0;
    seq_t v = val;
    if (v == 0) return false;
    while (v > 0) {
        int d = (int)(v % 9);
        if (d == 0)
            ++cnt0;
        else if (d == 6)
            ++cnt6;
        v /= 9;
    }
    return (cnt0 == cnt6) && (cnt0 > 0);
}

#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
#endif
static idx_t checked_max = 0;

static void cache_append(seq_t val)
{
#ifdef __cplusplus
    cache.push_back(val);
#else
    if (cache_sz == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failure – ignore silently */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
#endif
}

extern seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */

    /* extend cache until we have at least n terms */
    while (1) {
#ifdef __cplusplus
        if (cache.size() >= (size_t)n) break;
#else
        if (cache_sz >= (size_t)n) break;
#endif
        ++checked_max;
        seq_t cand = (seq_t)checked_max;
        if (a039362_valid(cand))
            cache_append(cand);
    }

    /* return the n‑th term (1‑based indexing) */
#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
