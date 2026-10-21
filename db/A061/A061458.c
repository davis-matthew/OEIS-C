/*
 * A061458  Cubes whose digit reversal is also a cube.
 * https://oeis.org/A061458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_cube_root, reverse, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed: cubes whose decimal digit reversal is also a cube */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_pow.h"

/* The sequence A061458: cubes whose digit reversal is also a cube. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_i = 0;      /* next integer to test */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_i = 0;
    }

    /* generate terms until we have at least n+1 terms */
    while (cache_len <= n) {
        /* compute i^3 */
        seq_t base = (seq_t)next_i;
        seq_t cube = seq_pow(base, (idx_t)3);

        /* reverse its decimal digits and test if that is a cube */
        seq_t rev = reverse(cube);
        seq_t root = floor_cube_root(rev);
        if (seq_pow(root, (idx_t)3) == rev) {
            /* store cube in cache, resizing if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                for (size_t i = 0; i < cache_len; i++)
                    new_cache[i] = cache[i];
                free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cube;
        }
        next_i++;
    }

    return cache[n];
}
