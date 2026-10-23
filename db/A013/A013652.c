/*
 * A013652  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains at least three 1's.
 * https://oeis.org/A013652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that the period of the continued fraction of
 *            sqrt(k) contains at least three 1's
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_ones_in_period, seq_floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Auxiliary functions for arbitrary‑precision seq_t.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_ones_in_period.h"
#include "../utils/seq_floor_sqrt.h"

/*  Cache to store previously generated terms.  */
static seq_t *cache   = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/*  Ensure the cache contains at least 'needed' slots.  */
static void cache_reserve(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/*  Next integer to examine for the continued‑fraction property.  */
static seq_t next_k = 2;

/*  Main entry for the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0)                     /* out of range, not used by OEIS */
        return 0;

    while (cache_size < (size_t)n)   /* generate up to n terms */
    {
        seq_t k;
        for (;;)
        {
            k = next_k++;
            /* skip perfect squares */
            if (count_ones_in_period(k) >= 3)
                break;
        }
        cache_reserve(cache_size + 1);
        cache[cache_size++] = k;
    }
    return cache[n - 1];
}
