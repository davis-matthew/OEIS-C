/*
 * A045290  Numbers whose base-5 representation contains exactly three 2's and
 *          one 4.
 * https://oeis.org/A045290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   has_three_twos_one_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045290: numbers whose base‑5 representation contains
   exactly three 2's and one 4.  The function searches sequentially,
   caches values, and returns the n‑th term (a(1) is the first). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_three_twos_one_four.h"

/* --- cache for memoisation ----------------------------------------- */
static seq_t *cache = NULL;      /* array of computed terms                */
static size_t cache_size = 0;    /* number of terms stored                 */
static size_t cache_cap  = 0;    /* current capacity of the array          */

/* --- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* undefined for n = 0, but we return 0 silently */
    if (n == 0)
        return (seq_t)0;

    /* expand the cache until it contains at least n terms           */
    if ((size_t)n > cache_size) {
        /* start from the next integer after the last cached term   */
        seq_t candidate = cache_size == 0 ? (seq_t)1
                                           : cache[cache_size - 1] + (seq_t)1;

        size_t target = (size_t)n;
        while (cache_size < target) {
            if (has_three_twos_one_four(candidate)) {
                /* grow the array if necessary */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = candidate;
            }
            /* next integer to test */
            candidate = candidate + (seq_t)1;
        }
    }
    return cache[n - 1];
}
