/*
 * A153171  First differences of A046163.
 * https://oeis.org/A153171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 218, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) -a(n-2) + 2a(n-3) + 2a(n-4) + 2a(n-5) - a(n-6) - a(n-7) - a(n-8)
   Initial values: a(0)=6, a(1)=6, a(2)=-6, a(3)=24, a(4)=12, a(5)=-24, a(6)=54, a(7)=18 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of computed entries */
static size_t cache_capacity = 0; /* allocated capacity */

static void init_cache(void) {
    if (cache != NULL) return;
    /* allocate capacity for first 8 terms */
    cache_capacity = 8;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    if (cache == NULL) { abort(); }
    cache[0] = (seq_t)6;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)-6;
    cache[3] = (seq_t)24;
    cache[4] = (seq_t)12;
    cache[5] = (seq_t)-24;
    cache[6] = (seq_t)54;
    cache[7] = (seq_t)18;
    cache_size = 8;
}

static void ensure_capacity_upto(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) { abort(); }
    cache = new_cache;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    init_cache();
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }
    /* Ensure we have room for all requested indices. */
    ensure_capacity_upto(n);
    /* Compute values up to n. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t val = -cache[(size_t)i-1] - cache[(size_t)i-2]
                    + (seq_t)2 * cache[(size_t)i-3]
                    + (seq_t)2 * cache[(size_t)i-4]
                    + (seq_t)2 * cache[(size_t)i-5]
                    - cache[(size_t)i-6]
                    - cache[(size_t)i-7]
                    - cache[(size_t)i-8];
        cache[(size_t)i] = val;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
