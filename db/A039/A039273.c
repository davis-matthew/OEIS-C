/*
 * A039273  Numbers whose base-12 representation has the same number of 9's
 *          and 11's.
 * https://oeis.org/A039273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   equal_9_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-12 representation has the same number of 9's and 11's
   Computation: iterate integers, keep those where the count of digit 9 equals
   the count of digit 11 in base 12. Results are cached so repeated calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/equal_9_11.h"

/* Cached sequence values: cache[i] holds a(i+1) */
static seq_t *cache     = NULL;
static size_t cache_sz  = 0;
static size_t cache_cap = 0;
static seq_t next_val   = 0;     /* next integer to test */
static idx_t  nums_found = 0;    /* number of terms found and stored */

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) == 0 */
    if (n == 0) return 0;   /* caller should not request n==0, but handle gracefully */

    /* Initial allocation */
    if (cache == NULL) {
        cache_cap = 16;
        cache_sz  = 0;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure, return 0 */
    }

    /* Expand cache as needed */
    while ((idx_t)nums_found < n) {
        if (equal_9_11(next_val)) {
            if (cache_sz >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (cache == NULL) return 0;   /* allocation failure */
            }
            cache[cache_sz++] = next_val;
            ++nums_found;
        }
        ++next_val;
    }

    return cache[n-1];
}
