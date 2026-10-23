/*
 * A032378  Noncubes k that are divisible by floor(k^(1/3)).
 * https://oeis.org/A032378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 853, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032378: Noncubes k that are divisible by floor(k^(1/3)). */
#include <stddef.h>
#include "../utils/floor_cube_root.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1; undefined for n < 1 */
        return 0;

    /* static cache of already-found terms */
    static seq_t *terms  = NULL;   /* array of found k values     */
    static idx_t  used   = 0;      /* number of terms stored     */
    static idx_t  cap    = 0;      /* capacity of the array      */
    static seq_t  next_k = 1;      /* next integer to test       */

    /* return cached value if already available */
    if (used >= n)
        return terms[n - 1];

    /* generate terms until the requested index is reached */
    while (used < n) {
        seq_t rc = floor_cube_root(next_k);

        /* skip perfect cubes */
        if (rc * rc * rc != next_k) {
            /* check divisibility */
            if (next_k % rc == 0) {
                /* grow the cache if necessary */
                ensure_capacity(used + 1, &cap,
                                 (void **)&terms, sizeof(seq_t));
                terms[used++] = next_k;
            }
        }
        ++next_k;
    }

    return terms[n - 1];
}
