/*
 * A205433  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          the same number of equal edges, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A205433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 21*a(n-1) - 156*a(n-2) + 442*a(n-3)
 *            - 81*a(n-4) - 1359*a(n-5) + 1134*a(n-6). Initial values (offset
 *            = 1): a(1)=15, a(2)=77, a(3)=443, a(4)=2705, a(5)=16955,
 *            a(6)=108173. The recurrence holds for all n ≥ 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for previously computed terms.  
   cache[i] stores a(i+1).  cache_size is number of stored terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined below the offset; return 0 */
        return (seq_t)0;
    }

    /* Ensure cache has at least n terms */
    if ((idx_t)cache_size < n) {
        /* Allocate new array of size n and copy old values */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        if (!new_cache) {
            /* Allocation failure: return 0 (unlikely in test environment) */
            return (seq_t)0;
        }
        for (idx_t i = 0; i < (idx_t)cache_size; ++i) {
            new_cache[i] = cache[i];
        }

        /* Initial values for n = 1..6 (stored at indices 0..5) */
        static const seq_t init[6] = {
            (seq_t)15,
            (seq_t)77,
            (seq_t)443,
            (seq_t)2705,
            (seq_t)16955,
            (seq_t)108173
        };

        /* Compute remaining terms up to n */
        for (idx_t i = (idx_t)cache_size; i < n; ++i) {
            if (i < 6) {
                new_cache[i] = init[i];
            } else {
                seq_t val =
                      (seq_t)21   * new_cache[i-1]
                    + (seq_t)442  * new_cache[i-3]
                    + (seq_t)1134 * new_cache[i-6]
                    - (seq_t)156  * new_cache[i-2]
                    - (seq_t)81   * new_cache[i-4]
                    - (seq_t)1359 * new_cache[i-5];
                new_cache[i] = val;
            }
        }

        /* Free old cache and replace with new one */
        free(cache);
        cache      = new_cache;
        cache_size = (size_t)n;
    }

    /* Return the requested term: a(n) is stored at cache[n-1] */
    return cache[n-1];
}
