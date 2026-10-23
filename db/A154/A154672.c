/*
 * A154672  Numbers n = 5*k^2 such that n - 1 and n + 1 are (twin) primes
 *          (thus k=6*m).
 * https://oeis.org/A154672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A154672:
   a(n) = 180 * m² where m = n, and n−1, n+1 are both prime.  */
/* The sequence is obtained by generating the numbers 180*m²
   for integer m≥1 and collecting those for which the integers
   180*m²−1 and 180*m²+1 are twin primes.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"

/* Static cache of already found terms. */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */
static uint64_t last_m   = 0;    /* last m that was tested */

/* Ensure the cache can hold at least “needed” elements. */
static void
ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();           /* memory exhaustion */

    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the nth element a(n) of the sequence (1‑based index). */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;   /* behaviour below offset does not matter */

    /* Generate terms until we have at least n stored. */
    while (cache_len < (size_t)n)
    {
        ++last_m;
        /* Compute 180 * m².  All arithmetic is performed in seq_t. */
        seq_t m_sq   = (seq_t)last_m * (seq_t)last_m;   /* m² */
        seq_t cand   = (seq_t)180 * m_sq;              /* 180*m² */

        /* Test twin primality of cand−1 and cand+1. */
        if (isPrimeGeneric(cand - (seq_t)1) &&
            isPrimeGeneric(cand + (seq_t)1))
        {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
