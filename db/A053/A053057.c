/*
 * A053057  Squares whose digit sum is also a square.
 * https://oeis.org/A053057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 81, u32 n <= 1192, u64 n <= 1192,
 *            u128 n <= 1192, big any n
 * Helpers:   digit_sum_seq, floor_sqrt, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squares whose digit sum is also a square.
   Computation: enumerate n^2 for n = 0,1,2,... and keep those
   where digit_sum_seq(n^2) is a perfect square (checked with
   floor_sqrt).  Results are cached in a static array so that
   calls for arbitrary n are O(n) once the list has been built. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum_seq.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* A small guard for the off‑by‑one offset; the OEIS offset is 1. */
    if (n == 0) return 0;

    /* Static cache of found squares */
    static seq_t *cache = NULL;        /* array of squares */
    static size_t cache_size = 0;      /* allocated capacity */
    static size_t terms = 0;           /* number of squares stored */
    static idx_t next_i = 0;           /* next integer to square */

    /* Grow cache as needed */
    while (terms < n) {
        if (terms >= cache_size) {
            size_t new_size = cache_size ? cache_size * 2 : 64;
            seq_t *tmp = seq_alloc_new_array(new_size);
            for (size_t i = 0; i < terms; ++i) tmp[i] = cache[i];
            if (cache) seq_free_array(cache);
            cache      = tmp;
            cache_size = new_size;
        }

        /* Test the next square */
        seq_t sq = (seq_t)next_i * (seq_t)next_i;
        seq_t ds = digit_sum_seq(sq);
        seq_t r  = floor_sqrt(ds);
        if (r * r == ds) {
            cache[terms++] = sq;
        }
        ++next_i;
    }

    return cache[n - 1];
}
