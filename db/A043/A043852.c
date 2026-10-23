/*
 * A043852  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 2 mod 8.
 * https://oeis.org/A043852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   count_runs_in_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043852: numbers k such that the number of runs in the
 * base‑4 representation of k is congruent to 2 (mod 8).
 *
 * The function performs an incremental search: it keeps a static
 * cache of already produced terms and iterates over natural numbers
 * until the required index is reached.  The search is O(n) and
 * satisfies the MS‐1 deadline for n≈10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/count_runs_in_base4.h"

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_size = 0;   /* how many terms are in cache */
static size_t cache_cap  = 0;   /* capacity of the array          */
static idx_t next_k = 1;        /* next integer to examine       */

/* Grow the cache capacity so that it can store at least
 * `need` more elements.  Uses realloc because seq_t is
 * already a POD type, even for arbitrary‐precision builds. */
static void cache_grow(size_t need)
{
    if (cache_cap == 0) cache_cap = 8;
    while (cache_cap < cache_size + need) cache_cap <<= 1;   /* double */
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);                           /* safety */
    cache = tmp;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, harmless */

    /* extend cache until it contains n terms */
    while (cache_size < (size_t)n) {
        /* examine next integer */
        idx_t k = next_k++;

        /* compute run count in base‑4, check condition */
        if ((count_runs_in_base4(k) % 8) == 2) {
            cache_grow(1);
            cache[cache_size++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
