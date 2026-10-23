/*
 * A039955  Squarefree numbers congruent to 1 (mod 4).
 * https://oeis.org/A039955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = least squarefree number congruent to 1 (mod 4) in order
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t current_candidate = (seq_t)0;
static bool initialized = false;

static void init(void) {
    cache_cap = 128;
    cache = seq_alloc(cache_cap);
    cache_len = 0;
    current_candidate = (seq_t)1;
    initialized = true;
}

static void grow_cache(void) {
    size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (!initialized) init();

    while (cache_len < (size_t)n) {
        if (is_squarefree(current_candidate)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = current_candidate;
        }
        current_candidate += (seq_t)4;
    }
    return cache[(size_t)(n - 1)];
}
