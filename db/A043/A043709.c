/*
 * A043709  Numbers whose base-6 representation has an odd number of runs.
 * https://oeis.org/A043709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base‑6 representation has an odd
 *            number of runs
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_run.h"

/* Cached sequence values.  Cached values are kept in increasing order. */
static seq_t *cache = NULL;      /* array of sequence values */
static idx_t  count = 0;         /* number of values stored in cache   */
static idx_t  cap   = 0;         /* capacity of the cache array        */
static seq_t  next_candidate = 1;/* next integer to test              */

/* Ensure the cache contains the value a(n) for every n <= target. */
static void extend_cache(idx_t target)
{
    while (count < target) {
        seq_t val = next_candidate;

        if (odd_run(val)) {
            /* Add to cache */
            if (count >= cap) {
                /* Double the capacity, or set to an initial value. */
                size_t new_cap = cap ? cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort program. */
                    abort();
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = val;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but return 0 */
    extend_cache(n);
    return cache[n - 1];
}
