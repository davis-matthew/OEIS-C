/*
 * A041771  Denominators of continued fraction convergents to sqrt(406).
 * https://oeis.org/A041771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   grow_seq_array, init_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// A041771: Denominators of continued fraction convergents to sqrt(406)
// Formula (for n > 27):
//     a(n) = 118936190 * a(n-14) - a(n-28)
// Initial values a(0)..a(27) are generated from the continued fraction
// of sqrt(406) (length‑14 period: 20; 6,1,2,4,7,1,4,1,7,4,2,1,6,40).
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_seq_array.h"
#include "../utils/init_terms.h"

/* global cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static bool cache_initialized = false;
static seq_t init_cache[28];

seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (!cache_initialized) {
        init_terms(init_cache);
        cache_initialized = true;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t required = (size_t)n + 1;
        if (required > cache_cap)
            grow_seq_array(&cache, &cache_cap, required);

        /* fill new entries */
        for (size_t i = cache_size; i < required; ++i) {
            if (i < 28) {
                cache[i] = init_cache[i];
            } else {
                /* recurrence: a(n) = 118936190*a(n-14) - a(n-28) */
                cache[i] = (seq_t)118936190 * cache[i - 14] - cache[i - 28];
            }
        }
        cache_size = required;
    }
    return cache[n];
}
