/*
 * A032632  Primes that are decimal concatenations of n with n + 9.
 * https://oeis.org/A032632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime obtained by concatenating an integer k
 *            with k+9 (i.e. seq_t concat_parts(k, k+9) is prime)
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 2317, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   alloc_seq_array, concat_parts, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/concat_parts.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/alloc_seq_array.h"

/* Static cache of found primes */
static seq_t  *cache      = NULL;   /* array of primes */
static idx_t  cache_cap  = 0;      /* allocated capacity */
static idx_t  cache_len  = 0;      /* number of primes stored */
static idx_t  next_k     = 1;      /* next k to test */

/* Ensure cache can hold at least 'need' elements.  Memory is allocated
   with `alloc_seq_array`; old memory is left unreclaimed (small leak is
   harmless for the problem size). */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined below offset */

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute terms until we reach index n. */
    while (cache_len < n) {
        while (1) {
            seq_t cand = concat_parts(next_k, next_k + 9);
            if (isPrimeGeneric(cand)) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = cand;
                ++next_k;                      /* advance to next k */
                break;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
