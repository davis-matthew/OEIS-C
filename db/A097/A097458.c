/*
 * A097458  The value of C in y = x^2+11x+C such that y is prime for all x = 0
 *          to 6.
 * https://oeis.org/A097458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097458 */
/* a(n) = the n-th integer C such that x^2+11x+C is prime for all integers x = 0…6. */
/* Searching by brute‑force. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* The offsets x^2+11x for x = 0…6. */
static const seq_t offsets[7] = {0, 12, 26, 42, 60, 80, 102};

/* Cache of found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next candidate C to test. */
static bool init_done = false;
static seq_t next_candidate;

/* Helper to grow the cache.  C++ requires a cast from void*. */
static void grow_cache(size_t min_cap) {
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether all 7 values are prime for a given candidate C. */
static bool candidate_ok(seq_t C) {
    for (size_t i = 0; i < 7; ++i) {
        if (!isPrimeGeneric(C + offsets[i])) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined below offset; return 0 */

    if (!init_done) {
        next_candidate = (seq_t)3;   /* start from the first odd >2 */
        init_done = true;
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Skip composite candidates for C itself. */
        if (!isPrimeGeneric(next_candidate)) {
            next_candidate += (seq_t)2;
            continue;
        }
        if (candidate_ok(next_candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        next_candidate += (seq_t)2;
    }

    return cache[(size_t)n - 1];
}
