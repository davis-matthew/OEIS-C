/*
 * A062316  Neither the sum nor difference of 2 squares.
 * https://oeis.org/A062316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 2507, u32 n <= 2507, u64 n <= 2507,
 *            u128 n <= 2507, big any n
 * Helpers:   is_sum_of_two_squares_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062316: a(n) = 2*k where k is the (odd) integer NOT representable
 * as a sum of two squares.  Numbers congruent to 2 (mod 4) that are
 * neither a sum nor a difference of two squares. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_sum_of_two_squares_uint64.h"

/* Memoised generation of the sequence up to the largest requested index. */
static seq_t *cache = NULL;          /* cached sequence values          */
static size_t cache_cap = 0;         /* capacity of cache array         */
static size_t cache_size = 0;        /* number of values stored         */
static uint64_t next_k = 1;          /* next odd integer to test (k)    */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset‑1 handling               */

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Test current odd k.  If it is NOT a sum of two squares,
         * then 2*k belongs to the sequence. */
        if (!is_sum_of_two_squares_uint64(next_k)) {
            /* Ensure enough space in the cache. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);          /* allocation failure             */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)(next_k * 2u);
        }
        next_k += 2;                  /* move to the next odd integer   */
    }

    return cache[n - 1];
}
