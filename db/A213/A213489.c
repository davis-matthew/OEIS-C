/*
 * A213489  Number of (w,x,y) with all terms in {0,...,n} and |w-x| + |x-y| +
 *          |y-w| >= w + x + y.
 * https://oeis.org/A213489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) - a(n-6)
 * with initial values
 * a(0)=1, a(1)=7, a(2)=19, a(3)=37, a(4)=64, a(5)=103
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of allocated elements */

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0; /* allocation failure – caller should not happen */

        /* copy existing data */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }

        /* initialise missing base values (0..5) */
        if (new_cache) {
            if (new_cap > 0)   new_cache[0] = 1;
            if (new_cap > 1)   new_cache[1] = 7;
            if (new_cap > 2)   new_cache[2] = 19;
            if (new_cap > 3)   new_cache[3] = 37;
            if (new_cap > 4)   new_cache[4] = 64;
            if (new_cap > 5)   new_cache[5] = 103;
        }

        /* compute remaining terms using the recurrence */
        idx_t start = (cap < 6) ? 6 : (idx_t)cap;   /* start from next undiscovered index */
        for (idx_t i = start; i <= n; ++i) {
            seq_t s1 = 4 * new_cache[i-1] + 8 * new_cache[i-3] + 4 * new_cache[i-5];
            seq_t s2 = 7 * new_cache[i-2] + 7 * new_cache[i-4] + new_cache[i-6];
            new_cache[i] = s1 - s2;   /* recurrence guarantees s1 >= s2 */
        }

        cache = new_cache;
        cap   = new_cap;
    }

    return cache[n];
}
