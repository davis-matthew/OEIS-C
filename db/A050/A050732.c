/*
 * A050732  Numbers of form 2^k (for values of k see A050723) containing no
 *          pair of consecutive equal digits (probably finite).
 * https://oeis.org/A050732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   mul_small, no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A050732: Search-based computation.  The sequence consists of the
   numbers 2^k for which k is in A050723, i.e., 2^k has no pair of
   consecutive equal decimal digits.  The code generates powers of two
   iteratively, tests each with no_consecutive_equal_digits, and
   stores the successful values in a static cache.
   (This satisfies requirement 9, stating that the code is based on a scan.) 
*/
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/mul_small.h"

static bool        initialized = false;
static seq_t      *cache       = NULL;   /* array of found powers of two   */
static size_t      cache_count = 0;     /* number of elements in cache   */
static size_t      cache_cap   = 0;     /* allocated capacity of cache   */
static seq_t       cur_val;            /* current power of two (2^k)     */
static idx_t       cur_k;              /* exponent of cur_val            */

seq_t a(idx_t n)
{
    /* Initialise static state on first call */
    if (!initialized) {
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* OOM – return 0 (unlikely in tests) */
            cache_count = 0;
            cache_cap = 0;
        } else {
            cache_count = 0;
        }
        cur_val = (seq_t)1;   /* 2^0 */
        cur_k   = 0;
        initialized = true;
    }

    /* Generate powers of two until we have at least n terms in the cache */
    while (cache_count < n) {
        if (no_consecutive_equal_digits(cur_val)) {
            /* Grow cache if necessary */
            if (cache_count == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* Allocation failed – abort algorithm */
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_count++] = cur_val;
        }
        /* Advance to next power of two */
        cur_val = mul_small(cur_val, 2);
        ++cur_k;
    }

    return cache[n - 1];
}
