/*
 * A039585  Numbers whose base-6 representation has the same number of 1's,
 *          2's and 5's.
 * https://oeis.org/A039585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: numbers whose base-6 representation has equal counts of 1, 2, and 5. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* Array holding computed terms (1-based). */
static idx_t cache_len = 0;          /* Current allocated length of cache. */
static idx_t found = 0;              /* Number of terms that have been found. */
static seq_t candidate = 0;          /* Next integer to test. */

static bool balanced(seq_t val) {
    unsigned cnt1 = 0, cnt2 = 0, cnt5 = 0;
    seq_t x = val;
    if (x == 0) return true;  /* 0 has zero counts for 1,2,5. */
    while (x > 0) {
        unsigned d = (unsigned)(x % 6);
        switch (d) {
            case 1: ++cnt1; break;
            case 2: ++cnt2; break;
            case 5: ++cnt5; break;
            default: break;
        }
        x /= 6;
    }
    return (cnt1 == cnt2) && (cnt2 == cnt5);
}

seq_t a(idx_t n) {
    /* Ensure cache is big enough for index n. */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* Allocation failed; return 0 as safe value. */
        if (cache) {
            for (idx_t i = 1; i <= found; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_len = new_len;
    }

    /* Generate terms up to n if needed. */
    while (found < n) {
        if (balanced(candidate)) {
            ++found;
            cache[found] = candidate;
        }
        ++candidate;
    }

    return cache[n];
}
