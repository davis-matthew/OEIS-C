/*
 * A061811  Multiples of 3 with all even digits.
 * https://oeis.org/A061811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   all_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function generates the sequence of multiples of 3 whose decimal digits are all even.
    It does so by iterating over successive multiples of 3 and testing each for the digit property.
    The sequence starts with 0 (a(1)=0), then 6, 24, 42, …
    The implementation caches computed terms so that subsequent calls reuse
    previously computed values without recomputing. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/all_even_digits.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;       /* cached sequence values */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t max_n = 0;          /* number of terms stored (max index computed) */
    static seq_t next_candidate = 0; /* next multiple of 3 to test */
    static bool initialized = false;

    /* Helper to grow cache if needed. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
    }

    /* Initialize on first call. */
    if (!initialized) {
        cache[0] = (seq_t)0;       /* a(1) = 0 */
        max_n = 1;
        next_candidate = (seq_t)3; /* next multiple of 3 to test */
        initialized = true;
    }

    if (n <= max_n) {
        return cache[n - 1];
    }

    /* Expand cache if needed. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate terms up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Find next multiple of 3 with all even digits. */
        seq_t cand = next_candidate;
        while (!all_even_digits(cand)) cand += (seq_t)3;
        cache[i - 1] = cand;
        next_candidate = cand + (seq_t)3;
    }

    max_n = n;
    return cache[n - 1];
}
