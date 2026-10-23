/*
 * A044911  Numbers whose base-10 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   alt_run_even_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A044911: numbers whose base‑10 run lengths alternate odd, even, odd, ... starting with odd. */
/* The routine scans integers from 1 upwards and stores each qualifying number in a cache. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alt_run_even_odd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_size = 0;
    static size_t cached_cap   = 0;
    static seq_t next_candidate = (seq_t)1;

    if (n == 0)      /* undefined for n < 1, but return 0 */
        return (seq_t)0;

    /* Extend cache up to index n */
    while (cached_size < (size_t)n) {
        if (alt_run_even_odd(next_candidate)) {
            /* Ensure capacity */
            if (cached_size == cached_cap) {
                size_t new_cap = (cached_cap == 0) ? 256 : cached_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* Memory exhausted */
                cache = tmp;
                cached_cap = new_cap;
            }
            cache[cached_size++] = next_candidate;
        }
        next_candidate += (seq_t)1;
    }
    return cache[n - 1];
}
