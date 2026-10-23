/*
 * A044344  Numbers n such that string 1,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044344: Return the nth integer whose decimal representation contains the
 * substring "12" while the preceding integer does not. */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: true if decimal representation of x contains "12". */
static bool contains12(seq_t x) {
    while (x >= 10) {
        if (x % 100 == 12)
            return true;
        x /= 10;
    }
    return false;
}

/* Cached sequence so that successive calls are fast. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

seq_t a(idx_t n) {
    if (n == 0)                /* graceful handling of the offset */ return 0;

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        seq_t candidate = (cache_size == 0) ? 12 : cache[cache_size - 1] + 1;

        /* Find the next number satisfying the definition. */
        while (!(contains12(candidate) && !contains12(candidate - 1)))
            candidate++;

        /* Grow cache if necessary. */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = candidate;
    }

    return cache[n - 1];
}
