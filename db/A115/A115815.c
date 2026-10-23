/*
 * A115815  Integers i such that 2*i XOR 7*i = 9*i.
 * https://oeis.org/A115815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers i such that (i*2) XOR (i*7) = i*9 */
#include <stdlib.h>

/* Static cache of computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t next_i = 0;
static idx_t count = 0;

/* Grow the cache to at least 'min_cap' elements */
static void ensure_cache(size_t min_cap) {
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < min_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (count < needed) {
        /* Find more elements until we have enough */
        while (count < needed) {
            seq_t x = (seq_t)next_i;
            if (((x * 2) ^ (x * 7)) == (x * 9)) {    /* test condition */
                ensure_cache(count + 1);
                cache[count++] = x;
            }
            next_i++;
        }
    }
    return cache[(size_t)n];
}
