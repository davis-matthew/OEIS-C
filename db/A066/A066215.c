/*
 * A066215  Numbers which are sums of cubes of some subset of divisors.
 * https://oeis.org/A066215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 506, u32 n <= 9197, u64 n <= 9197,
 *            u128 n <= 9197, big any n
 * Helpers:   dfs_subset_sum, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066215: Numbers that are sums of cubes of a subset of their divisors.
   The routine tests each integer m in increasing order, keeps a cache of
   all numbers that satisfy the property, and returns the n‑th such
   integer.                                              */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/int_cube_root.h"
#include "../utils/dfs_subset_sum.h"

/* --------------------------------------------------------------------- */
/* Recursive subset–sum search with pruning                               */

/* --------------------------------------------------------------------- */
/* Test if a given integer m belongs to the sequence                     */
/* --------------------------------------------------------------------- */
static bool in_sequence(idx_t m)
{
    if (m == 0) return false;

    /* Any perfect cube is automatically in the sequence.  */
    idx_t cbrt = int_cube_root(m);
    if ((seq_t)cbrt * cbrt * cbrt == m) return true;

    /* Collect cubes of divisors d <= cbrt(m).                        */
    /* The maximum number of such divisors for moderately sized m is tiny. */
    seq_t cubes[256];
    size_t len = 0;
    seq_t sum_of_cubes = 0;

    for (idx_t d = 1; d <= cbrt; ++d) {
        if (m % d == 0) {
            seq_t cube = (seq_t)d * d * d;
            cubes[len++] = cube;
            sum_of_cubes += cube;
            if (len == sizeof(cubes) / sizeof(cubes[0]))
                break;   /* safety, though unlikely to hit */
        }
    }

    /* If the total of all cubes is smaller than m, impossible.         */
    if (sum_of_cubes < (seq_t)m) return false;

    /* Sort cubes in descending order to prune early.                      */
    for (size_t i = 1; i < len; ++i) {
        seq_t key = cubes[i];
        size_t j = i;
        while (j > 0 && cubes[j - 1] < key) {
            cubes[j] = cubes[j - 1];
            --j;
        }
        cubes[j] = key;
    }

    /* Cumulative sums of the remaining cubes for pruning.               */
    seq_t cum_rem[257];
    cum_rem[len] = 0;
    for (size_t i = len; i-- > 0; ) {
        cum_rem[i] = cum_rem[i + 1] + cubes[i];
    }

    /* Search for a subset that sums exactly to m.                        */
    return dfs_subset_sum(0, 0, cubes, cum_rem, (seq_t)m, len);
}

/* --------------------------------------------------------------------- */
/* Cache of discovered terms                                               */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored      */
static size_t cache_cap  = 0;   /* capacity of the cache array */
static idx_t next_candidate = 1;/* next integer to test        */

/* --------------------------------------------------------------------- */
/* Main accessor:  a(n)  returns the n‑th term (1‑based index).           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for index 0 */

    if (cache == NULL) {       /* initialise cache */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have enough.                               */
    while (cache_size < n) {
        idx_t m = next_candidate;

        if (in_sequence(m)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;                /* double the capacity */
                cache = (seq_t*)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)m;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
