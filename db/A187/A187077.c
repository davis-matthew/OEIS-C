/*
 * A187077  Number of row-convex polyplets with n cells.
 * https://oeis.org/A187077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187077: Number of row-convex polyplets with n cells
   Recurrence: a(n) = 7*a(n-1) - 13*a(n-2) + 10*a(n-3) - 2*a(n-4)  for n>4
   Initial values: a(1)=1, a(2)=4, a(3)=18, a(4)=83
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms, 1‑indexed */
static idx_t   cache_max = 0;        /* largest n for which cache[n] is valid */

/* Return the n‑th term (n >= 1).  Cached values are stored to allow
   arbitrary order of calls.  The recurrence is implemented using
   non‑negative intermediate values to avoid underflow on unsigned
   seq_t. */
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (cache == NULL || cache_max < n) {
        size_t new_cap = (size_t)n + 1;          /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(cache,
                                       new_cap * sizeof(seq_t));
        if (tmp == NULL) {                       /* out of memory */
            /* In a contest environment we could exit or abort,
               but return 0 here for safety. */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise the initial four terms if they have not yet
           been set.  cache_max holds the largest index already
           initialised. */
        idx_t i;
        for (i = cache_max + 1; i <= (idx_t)4 && i <= n; ++i) {
            switch (i) {
            case 1: cache[1] = (seq_t)1;  break;
            case 2: cache[2] = (seq_t)4;  break;
            case 3: cache[3] = (seq_t)18; break;
            case 4: cache[4] = (seq_t)83; break;
            }
        }

        /* Compute further terms using the linear recurrence. */
        idx_t start = cache_max + 1;
        if (start < 5) start = 5;           /* recurrence is defined for n>4 */

        for (i = start; i <= n; ++i) {
            seq_t sum1 = cache[i - 1] * (seq_t)7
                       + cache[i - 3] * (seq_t)10;
            seq_t sum2 = cache[i - 2] * (seq_t)13
                       + cache[i - 4] * (seq_t)2;
            cache[i] = sum1 - sum2;
        }

        cache_max = n;                      /* update largest computed index */
    }

    return cache[n];
}
