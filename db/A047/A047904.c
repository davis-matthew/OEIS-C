/*
 * A047904  a(n+1) = a(n) + n (if n is odd), a(n+1) = a(n) * n (if n is even).
 * https://oeis.org/A047904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 34, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   for n >= 2, a(n) = a(n-1) + (n-1) if (n-1) is odd,
                     a(n-1) * (n-1) if (n-1) is even */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* array of computed terms, 0-indexed */
static size_t cache_cap = 0;     /* number of allocated slots */

/* compute a(n) using memoisation; cache grows as needed */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1 */

    /* Grow cache if necessary */
    if (cache_cap < (size_t)n) {
        size_t old_cap = cache_cap;
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)
            return 0;           /* allocation failure */

        cache = new_cache;
        cache_cap = n;

        /* Initialise first term if cache was empty */
        if (old_cap == 0) {
            cache[0] = (seq_t)1;
            old_cap = 1;
        }

        /* Compute entries that are now in range (old_cap+1 .. n) */
        for (size_t j = old_cap + 1; j <= (size_t)n; ++j) {
            idx_t step = (idx_t)(j - 1); /* corresponds to (n-1) in recurrence */
            seq_t prev = cache[j - 2];

            seq_t next;
            if ((step & 1) != 0)          /* odd step: addition */
                next = prev + (seq_t)step;
            else                          /* even step: multiplication */
                next = prev * (seq_t)step;

            cache[j - 1] = next;
        }
    }

    return cache[(size_t)n - 1];
}
