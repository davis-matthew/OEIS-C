/*
 * A181155  Odious numbers (A000069) plus one; complement of A026147.
 * https://oeis.org/A181155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = odious(n) + 1 where odious(n) is the n-th integer with an
 *            odd number of 1 bits in binary
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_odious_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_odious_u64.h"

/* Static cache of computed values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_num = 0;   /* integer just processed */

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n) {
    while (cache_len < n) {
        uint64_t num;
        /* Find the next odious number. */
        do {
            num = ++last_num;
        } while (!is_odious_u64(num));

        /* Convert to seq_t and store. */
        seq_t val = (seq_t)(num + 1);
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1024 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* memory allocation failed */
            cache   = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for this sequence, safe guard */
    ensure_cache(n);
    return cache[n - 1];
}
