/*
 * A212825  Number of 0..4 arrays of length n with no adjacent pair equal to
 *          its immediately preceding adjacent pair, and new values introduced
 *          in 0..4 order.
 * https://oeis.org/A212825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212825: Number of 0..4 arrays of length n with no adjacent pair equal to its immediately preceding adjacent pair,
   new values introduced in 0..4 order.
   Recurrence (for n > 9):
       a(n) = 7*a(n-1) - 7*a(n-2) - 20*a(n-3) + 10*a(n-4) + 24*a(n-5) + 8*a(n-6)
   Initial values:
       a(1)=1, a(2)=2, a(3)=5, a(4)=13, a(5)=44, a(6)=167, a(7)=695, a(8)=3070, a(9)=14074
*/

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* Handle undefined n */
    if (n == 0)
        return 0;

    /* Static memoisation table */
    static seq_t *cache      = NULL;   /* cache[i] stores a(i+1)  */
    static idx_t cached_size = 0;      /* number of terms stored */

    /* Pre‑computed first 9 terms */
    static const seq_t init[9] = {
        (seq_t)1,   (seq_t)2,   (seq_t)5,
        (seq_t)13,  (seq_t)44,  (seq_t)167,
        (seq_t)695, (seq_t)3070, (seq_t)14074
    };

    /* If we already computed this term, return it */
    if (n <= cached_size)
        return cache[n - 1];

    /* Need to extend the cache up to n */
    idx_t old_size = cached_size;
    seq_t *new_cache = seq_alloc_new_array(n);
    if (!new_cache)
        return 0;   /* allocation failure */

    /* Copy existing values */
    for (idx_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];

    /* Free old array if any */
    if (cache)
        seq_free(cache);

    /* Assign new cache */
    cache = new_cache;
    cached_size = n;

    /* Compute missing terms */
    for (idx_t i = old_size + 1; i <= n; ++i) {   /* i is 1‑based */
        if (i <= 9) {
            cache[i - 1] = init[i - 1];
            continue;
        }

        /* Use unsigned arithmetic:
           a(n) = 7*a(n-1) + 10*a(n-4) + 24*a(n-5) + 8*a(n-6)
                 - (7*a(n-2) + 20*a(n-3))
        */
        seq_t pos = (seq_t)7 * cache[i - 2]            /* a(n-1) */
                  + (seq_t)10 * cache[i - 5]           /* a(n-4) */
                  + (seq_t)24 * cache[i - 6]           /* a(n-5) */
                  + (seq_t)8  * cache[i - 7];          /* a(n-6) */

        seq_t neg = (seq_t)7 * cache[i - 3]            /* a(n-2) */
                  + (seq_t)20 * cache[i - 4];          /* a(n-3) */

        cache[i - 1] = pos - neg;
    }

    return cache[n - 1];
}
