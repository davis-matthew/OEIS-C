/*
 * A100528  a(0) = 1, a(n+1) = a(n)^2 + 1 - floor(log_2(a(n))).
 * https://oeis.org/A100528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n+1) = a(n)^2 + 1 - floor(log_2(a(n)))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* floor(log2(x)) for an unsigned seq_t value */
static inline seq_t floor_log2_seq(seq_t x)
{
    seq_t log = (seq_t)0;
    while (x > (seq_t)1) {
        x = x / (seq_t)2;
        ++log;
    }
    return log;
}

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t cache_size = 0;         /* number of cached entries */

seq_t a(idx_t n)
{
    /* Initialise cache with a(0) */
    if (cache_size == 0) {
        cache = mem_new(seq_t, 1);
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Extend cache if needed */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        seq_t prev = cache[old_size - 1];
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t log2 = floor_log2_seq(prev);
            seq_t next = prev * prev + (seq_t)1 - log2;
            cache[i] = next;
            prev = next;
        }
        cache_size = new_size;
    }
    return cache[n];
}
