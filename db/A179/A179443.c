/*
 * A179443  Numbers m such that m and 10*m + 1 are both composites.
 * https://oeis.org/A179443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179443: Numbers m such that m and 10*m + 1 are both composites.
   The function returns the nth such number by scanning integers and testing
   for compositeness using the provided is_prime routine.  Results are cached
   for fast subsequent access. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap    = 0;   /* allocated capacity of the cache   */
    static idx_t    cache_len = 0; /* number of cached values           */
    static idx_t    last_checked = 0; /* last integer examined            */

    /* Grow the cache so that cache[n-1] will be valid */
    while (cache_len < n) {
        /* Search for the next qualifying integer */
        for (idx_t m = last_checked + 1; ; ++m) {
            if (m > 1 && !is_prime(m)) {
                idx_t t = m * 10 + 1;
                if (!is_prime(t)) {
                    /* Ensure cache has space for the new entry */
                    if (cap <= cache_len) {
                        size_t new_cap = cap == 0 ? 1 : cap * 2;
                        if (new_cap <= cache_len)
                            new_cap = cache_len + 1;
                        seq_t *tmp = (cap == 0)
                            ? seq_alloc(new_cap)
                            : seq_realloc(cache, cap, new_cap);
                        cache = tmp;
                        cap   = new_cap;
                    }
                    /* Store the new value */
                    cache[cache_len++] = (seq_t)m;
                    last_checked = m;
                    break; /* proceed to fill next entry if needed */
                }
            }
        }
    }
    return cache[n - 1];
}
