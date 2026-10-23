/*
 * A030479  Cubes with property that all even digits occur together and all
 *          odd digits occur together.
 * https://oeis.org/A030479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A030478(n))^3, where A030478(n) is the n‑th positive
 *            integer whose cube has all even digits together and all odd
 *            digits together (decimal notation)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   parity_blocks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/parity_blocks.h"

/* Dynamic cache for the cubes of the accepted numbers. */
static seq_t *cache  = NULL;   /* array of cubes */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of cached terms */
static idx_t next_candidate = 0;   /* next n to test for inclusion */

/* Ensure that the cache can hold at least “need” entries. */
static void ensure_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}

/* Generate more terms of the sequence until at least n terms are cached. */
static void fill_cache(idx_t n)
{
    while (cache_len < n) {
        seq_t m = (seq_t)next_candidate;
        seq_t cube = m * m * m;
        if (parity_blocks(cube)) {
            ensure_cap(cache_len + 1);
            cache[cache_len++] = cube;
        }
        ++next_candidate;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* index n is 1‑based */
    fill_cache(n);
    return cache[n - 1];
}
