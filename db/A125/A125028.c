/*
 * A125028  a(1)=2, a(n)=2*a(n-1)-floor(sqrt(a(n-1))).
 * https://oeis.org/A125028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(n)=2*a(n-1)-floor_sqrt(a(n-1)) for n>=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq_array, floor_sqrt, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;   /* array indexed directly by n (0 unused) */
    static size_t cap   = 0;      /* current capacity of cache (>= len+1)   */
    static size_t len   = 0;      /* highest computed index (>=1 if len>0) */

    if (n == 0)                     /* undefined for n<1, return 0 as a safety */
        return 0;

    /* Ensure cache is prepared for at least index n */
    size_t N = (size_t)n;
    if (len < N) {
        /* Grow capacity if necessary (at least N+1 for index 0) */
        size_t required = N + 1;
        if (cap < required) {
            size_t new_cap = cap ? (cap << 1) : 4;
            while (new_cap < required)
                new_cap <<= 1;

            /* Allocate new array with proper construction (delegated to utils) */
            seq_t *new_cache = alloc_seq_array(new_cap);
            if (!new_cache)
                abort();           /* allocation failure */

            /* Copy existing elements */
            if (cap > 0) {
                for (size_t i = 0; i <= len; ++i)
                    new_cache[i] = cache[i];
                free_seq_array(cache);
            }
            cache = new_cache;
            cap   = new_cap;
        }

        /* Initialize first term if not done */
        if (len == 0) {
            cache[0] = (seq_t)0;          /* unused slot */
            cache[1] = (seq_t)2;          /* a(1)=2 */
            len = 1;
        }

        /* Compute missing terms up to N */
        for (size_t i = len + 1; i <= N; ++i) {
            seq_t prev = cache[i - 1];
            seq_t s    = floor_sqrt(prev);
            cache[i]   = prev * (seq_t)2 - s;
        }
        len = N;
    }
    return cache[(size_t)n];
}
