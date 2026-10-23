/*
 * A117803  Triangular numbers for which the sum of the digits is a cube.
 * https://oeis.org/A117803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular numbers whose sum of digits is a perfect cube. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Static cache for computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of values stored in cache */
static size_t cache_cap = 0;   /* current allocated capacity */
static idx_t last_k = 0;       /* index k used to generate triangular numbers */

/* Pre‑computed small cubes that can occur as a digit sum. */
static const unsigned int cubes[] = {0, 1, 8, 27, 64, 125, 216, 343};
#define NUM_CUBES (sizeof(cubes)/sizeof(cubes[0]))

/* Returns true if s is a perfect cube from the pre‑computed list. */
static inline bool is_cube(unsigned int s)
{
    for (size_t i = 0; i < NUM_CUBES; ++i)
        if (s == cubes[i]) return true;
    return false;
}

/* Grows the cache if needed to accommodate at least 'needed' elements. */
static void grow_cache_if_needed(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* a(n) returns the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) abort();           /* out of range; undefined behaviour */

    /* Ensure cache has room for at least n terms. */
    if (cache_len < (size_t)n) {
        grow_cache_if_needed((size_t)n);

        /* First value in the sequence is the triangular number T(0) = 0. */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache_len = 1;
            last_k = 0;
        }

        /* Generate triangular numbers until we have n terms. */
        while (cache_len < (size_t)n) {
            ++last_k;                         /* next k, starting at 1 */
            seq_t tri = (seq_t)last_k * ((seq_t)last_k + 1) / 2;
            seq_t sum = digit_sum(tri);       /* sum of digits of tri */
            /* digit_sum returns seq_t, but the sum is small; cast safely. */
            if (is_cube((unsigned int)sum)) {
                cache[cache_len++] = tri;
            }
        }
    }
    return cache[n - 1];
}
