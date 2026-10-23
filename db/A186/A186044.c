/*
 * A186044  Number of (n+2) X 3 binary arrays with each 3 X 3 subblock
 *          singular.
 * https://oeis.org/A186044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Empirical recurrence: a(n) = 5*a(n-1) + 4*a(n-2) + 18*a(n-3) 
   - 40*a(n-4) - 24*a(n-5) - 72*a(n-6)
   Initial values: a(1)=338, a(2)=2038, a(3)=12278, a(4)=72802, a(5)=434174, a(6)=2588842 */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;              /* out of range index */
    static seq_t *cache = NULL;              /* indexed from 0, 0 unused */
    static idx_t max_computed = 0;           /* largest n for which cache is valid */

    /* Initialise cache with the first 6 terms */
    if (!cache) {
        cache = alloc_seq_array(7);          /* indices 0 .. 6 */
        cache[0]   = 0;
        cache[1]   = 338;
        cache[2]   = 2038;
        cache[3]   = 12278;
        cache[4]   = 72802;
        cache[5]   = 434174;
        cache[6]   = 2588842;
        max_computed = 6;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* Allocate a larger cache to hold terms up to n */
    size_t new_len = (size_t)n + 1;          /* indices 0 .. n */
    seq_t *new_cache = alloc_seq_array(new_len);

    /* Copy known values into the expanded cache */
    for (idx_t i = 0; i <= max_computed; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms using the empirical recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        new_cache[i] = 5 * new_cache[i - 1]
                     + 4 * new_cache[i - 2]
                     + 18 * new_cache[i - 3]
                     - 40 * new_cache[i - 4]
                     - 24 * new_cache[i - 5]
                     - 72 * new_cache[i - 6];
    }

    /* Replace old cache with the expanded one */
    free_seq_array(cache);
    cache = new_cache;
    max_computed = n;

    return cache[n];
}
