/*
 * A092170  Sum of squares of alternating factorials : n!^2 - (n-1)!^2 +
 *          (n-2)!^2 - ... 1!^2.
 * https://oeis.org/A092170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!^2 - a(n-1), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types: seq_t, idx_t are defined by the build. */
static seq_t *a_cache = NULL;          /* a_cache[i-1] = a(i) */
static seq_t *fact2_cache = NULL;      /* fact2_cache[i-1] = (i!)^2 */
static idx_t cache_size = 0;           /* highest n computed */

seq_t a(idx_t n)
{
    /* Expand caches if necessary */
    if (n > cache_size) {
        idx_t new_size = n;
        /* realloc handles NULL pointers as well */
        a_cache     = (seq_t *)realloc(a_cache,     new_size * sizeof(seq_t));
        fact2_cache = (seq_t *)realloc(fact2_cache, new_size * sizeof(seq_t));

        /* Compute values from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* factorial squared: fact2[i] = fact2[i-1] * i * i */
            if (i == 1) {
                fact2_cache[i-1] = (seq_t)1;          /* 1!^2 == 1 */
            } else {
                fact2_cache[i-1] =
                    fact2_cache[i-2] * ((seq_t)i * (seq_t)i);
            }

            /* sequence recurrence */
            if (i == 1) {
                a_cache[i-1] = (seq_t)1;            /* a(1) == 1 */
            } else {
                a_cache[i-1] = fact2_cache[i-1] - a_cache[i-2];
            }
        }
        cache_size = n;
    }
    return a_cache[n-1];
}
