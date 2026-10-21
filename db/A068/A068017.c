/*
 * A068017  Composite n such that sigma(n) - 1 and sigma(n) + 1 are twin
 *          primes.
 * https://oeis.org/A068017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, isPrimeGeneric, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A068017: composite n such that sigma(n)-1
   and sigma(n)+1 are twin primes. The function searches for the
   required terms and caches them for subsequent calls. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/ensure_capacity.h"
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;   /* cached sequence values          */
static size_t cache_size = 0; /* number of cached terms          */
static size_t cache_cap = 0;  /* allocated cache capacity        */
static idx_t next_candidate = 4; /* next integer to test (composite) */

seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n=0, return 0 */
        return 0;

    if (n <= cache_size)            /* already computed */
        return cache[n - 1];

    /* Generate further terms until we have at least n of them */
    while (cache_size < n)
    {
        /* Skip primes – only composites are relevant */
        if (is_prime(next_candidate))
        {
            ++next_candidate;
            continue;
        }

        /* Compute sigma(n) */
        seq_t sigma = sigma_of(next_candidate);

        /* Check twin‑prime condition on sigma-1 and sigma+1 */
        seq_t s_minus = sigma - 1;
        seq_t s_plus  = sigma + 1;

        if (isPrimeGeneric(s_minus) && isPrimeGeneric(s_plus))
        {
            /* Ensure cache has room for another element */
            ensure_capacity(cache_size + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
