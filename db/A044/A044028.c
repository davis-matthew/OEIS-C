/*
 * A044028  Numbers k such that 4 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that digits 4 and 7 appear
 *            consecutively in the decimal representation of k but not in k+1.
 *            Computed by brute‑force search with memoisation
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent47_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent47_uint64.h"

/* check the property for a candidate k */
static bool satisfies(uint64_t k) {
    return has_adjacent47_uint64(k) && !has_adjacent47_uint64(k + 1);
}

/* memoised storage for the sequence */
static seq_t *cache = NULL;
static size_t cache_size = 0;    /* number of elements stored */
static size_t cache_capacity = 0;
static uint64_t checked = 0;     /* up to which integer has been examined */

/* ensure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need) {
    if (cache_capacity >= need)
        return;
    size_t new_capacity = cache_capacity ? cache_capacity * 2 : 16;
    while (new_capacity < need)
        new_capacity <<= 1;
    seq_t *new_mem = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    if (!new_mem) abort(); /* memory allocation failed */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_mem[i] = cache[i];
        free(cache);
    }
    cache = new_mem;
    cache_capacity = new_capacity;
}

/* append a new value to the cache */
static void append_to_cache(seq_t val) {
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = val;
}

seq_t a(idx_t n) {
    /* n is 1‑based; no need to handle n==0 for the OEIS offset */
    while (cache_size < (size_t)n) {
        ++checked;
        if (satisfies(checked))
            append_to_cache((seq_t)checked);
    }
    return cache[n - 1];
}
