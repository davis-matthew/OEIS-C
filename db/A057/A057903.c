/*
 * A057903  Positive integers that are not the sum of exactly two positive
 *          cubes.
 * https://oeis.org/A057903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057903: positive integers that are not the sum of exactly two
   positive cubes.  The function builds a cache of the terms and
   expands it on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

/* Helper to test whether v can be expressed as a sum of two positive cubes. */
static bool
is_sum_of_two_cubes(seq_t v)
{
    /* a^3 + b^3 = v with a,b >= 1.  Scan a up to the cube root of v. */
    idx_t a_max = (idx_t)floor_cube_root(v);   /* cube root of v */

    for (idx_t a = 1; a <= a_max; ++a) {
        seq_t a3 = (seq_t)a * a * a;
        seq_t rem = v - a3;
        if (rem <= 0) continue;               /* b would be 0 or negative */

        seq_t b = (seq_t)floor_cube_root(rem);
        /* check if rem is a positive perfect cube */
        if ((seq_t)b * b * b == rem)
            return true; /* found a representation */
    }
    return false;
}

/* Static cache of sequence values. */
static seq_t *cache      = NULL; /* array of terms */
static size_t cache_len  = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* allocated capacity */
static idx_t  max_checked = 0;   /* largest integer examined so far */

/* Expand the cache until it contains at least n terms. */
static void
ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return; /* already have enough */

    /* Simple linear scan from the next integer after max_checked. */
    for (idx_t m = max_checked + 1; cache_len < (size_t)n; ++m) {
        seq_t val = (seq_t)m;
        if (!is_sum_of_two_cubes(val)) {
            /* add to cache */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        max_checked = m;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for n < 1; return 0 for safety */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n - 1];
}
