/*
 * A050750  Cubes containing no pair of consecutive equal digits.
 * https://oeis.org/A050750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A050742(n)^3 (n‑th qualifying number cubed)
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits.h"

/* Static cache of generated cubes.  The cache is extended on demand. */
static seq_t *cube_cache = NULL;   /* a(k) for k = 0..cache_len-1   */
static size_t cache_len = 0;       /* number of cached terms         */
static size_t cache_cap = 0;       /* capacity of cube_cache         */
static idx_t next_m = 0;           /* next integer to test          */

seq_t a(idx_t n)
{
    /* Ensure that at least n terms have been generated. */
    while (cache_len < (size_t)n) {
        seq_t m = (seq_t)next_m;
        seq_t cube = m * m * m;                /* compute m³ in seq_t type */

        /* Store only cubes whose digits contain no consecutive equal pair. */
        if (no_consecutive_equal_digits(cube)) {
            if (cache_len == cache_cap) {      /* grow the cache if needed */
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cube_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cube_cache = tmp;
                cache_cap = new_cap;
            }
            cube_cache[cache_len++] = cube;     /* append qualifying cube   */
        }
        next_m++;                               /* advance to next candidate */
    }

    return cube_cache[n - 1];
}
