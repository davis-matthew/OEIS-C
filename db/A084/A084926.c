/*
 * A084926  Inverse hyperbolic cotangent reducible numbers. The hyperbolic
 *          analog of the non-Stormer numbers (A002312).
 * https://oeis.org/A084926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A084926 (Inverse hyperbolic cotangent reducible numbers).
   a(n) is the n-th integer m (≥ 5) for which there exists an integer  
   x with 1 < x < m such that (x·m + 1)/(x + m) is an integer.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: check if integer m satisfies the defining condition. */
static bool is_valid(idx_t m)
{
    /* search for a suitable x: 1 < x < m */
    for (idx_t x = 2; x < m; ++x) {
        idx_t num = x * m + 1;
        idx_t den = x + m;
        if (num % den == 0) {
            return true;
        }
    }
    return false;
}

/* The memoised sequence storage. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;  /* number of values stored */
static size_t cache_cap = 0;
static idx_t last_checked = 4;   /* the last m we examined */

/* Compute the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        idx_t m = ++last_checked;            /* next candidate (starts at 5) */

        if (is_valid(m)) {
            /* Ensure enough capacity. */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
                seq_t *new_cache =
                    (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* Allocation failed – abort or handle as needed. */
                    abort();
                }
                cache     = new_cache;
                cache_cap = new_cap;
            }

            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
