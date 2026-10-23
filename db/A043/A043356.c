/*
 * A043356  Numbers having four 1's in base 5.
 * https://oeis.org/A043356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2020, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_four_exact_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043356: numbers having exactly four 1's in base 5 */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_four_exact_ones.h"

/* Cache of previously found terms to allow arbitrary call order */
static seq_t *cache = NULL;
static size_t cache_used = 0;
static size_t cache_cap  = 0;
static idx_t  last_candidate = 1;      /* next integer to test */

/* Ensure the cache has room for one more entry */
static void ensure_capacity(void) {
    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_used + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
}

/* Generate terms until at least the nth term is stored */
static void generate_to(idx_t n) {
    while (cache_used < n) {
        if (has_four_exact_ones((seq_t)last_candidate)) {
            ensure_capacity();
            cache[cache_used++] = (seq_t)last_candidate;
        }
        last_candidate++;
    }
}

/* Public interface */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                      /* offset starts at 1, behaviour below 1 is irrelevant */
    generate_to(n);
    return cache[n - 1];
}
