/*
 * A140001  Primes of the form 5x^2+264y^2.
 * https://oeis.org/A140001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 209, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime of the form 5x^2 + 264y^2,
   where the first term (n = 1) is 5.  Subsequent primes are
   those that are congruent to one of the ten residues
   {221, 269, 389, 509, 581, 749, 1061, 1109, 1181, 1301} modulo 1320.  */
/* The function is memoised and works for any order of calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_array.h"

static const idx_t residues[10] = {
    221, 269, 389, 509,
    581, 749, 1061, 1109,
    1181, 1301
};

static seq_t *cache      = NULL;          /* cached sequence values       */
static idx_t  cache_size = 0;              /* number of cached values       */
static size_t cache_cap  = 0;              /* allocated capacity            */
static seq_t  next_val[10] = {0};          /* next candidate per residue    */
static bool   initialized = false;         /* init flag                     */

static void init_sequence(void)
{
    if (initialized) return;

    /* Allocate an initial cache for a few terms. */
    cache_cap = 64;
    cache     = seq_alloc_array(cache_cap);
    if (!cache) return;                     /* allocation failure: abort */

    /* First term is the special prime 5. */
    cache[0] = (seq_t)5;
    cache_size = 1;

    /* Initialise the candidate values for each residue class. */
    for (int i = 0; i < 10; ++i)
        next_val[i] = (seq_t)residues[i];

    initialized = true;
}

static void cache_append(seq_t val)
{
    if (cache_size + 1 > cache_cap) {
        size_t newcap = cache_cap * 2;
        seq_t *tmp = seq_alloc_array(newcap);
        if (!tmp) return;                   /* allocation failed       */
        memcpy(tmp, cache, sizeof(seq_t) * cache_size);
        free(cache);
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_size++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* unused; behaviour below offset is irrelevant */

    init_sequence();

    if (n <= cache_size) return cache[n - 1];

    while (cache_size < n) {
        /* Find the smallest candidate from the ten progressing lists. */
        idx_t best = 0;
        for (idx_t i = 1; i < 10; ++i)
            if (next_val[i] < next_val[best]) best = i;

        seq_t candidate = next_val[best];

        /* Test the candidate for primality. */
        if (is_prime_big(candidate))
            cache_append(candidate);

        /* Advance the chosen progression by the common difference 1320. */
        next_val[best] += (seq_t)1320;
    }

    return cache[n - 1];
}
