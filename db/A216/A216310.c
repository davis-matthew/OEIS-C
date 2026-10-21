/*
 * A216310  The prime ending in 3 is the only prime in a decade.
 * https://oeis.org/A216310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p = 10*k+3 such that 10*k+1, 10*k+7 and 10*k+9 are all
   composite.  The function builds a cache of the required primes and
   returns the n‑th one (n ≥ 1). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t) */
#include "../utils/seq_realloc.h"      /* seq_t* seq_realloc(seq_t*, size_t, size_t) */

static seq_t *cache = NULL;     /* array of found primes               */
static size_t cache_cap = 0;    /* allocated capacity (in seq_t units) */
static idx_t  cache_len = 0;    /* number of stored primes             */
static idx_t  next_k   = 0;     /* next k value to test                */

/* Ensure that cache has at least n elements. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Search for the next qualifying prime. */
        for (;;) {
            /* Candidate of the form 10*k+3. */
            seq_t cand = (seq_t)10 * (seq_t)next_k + (seq_t)3;

            /* Quick primality test on the candidate. */
            if (!isPrimeGeneric(cand)) {
                ++next_k;
                continue;
            }

            seq_t l1 = (seq_t)10 * (seq_t)next_k + (seq_t)1;
            seq_t l7 = (seq_t)10 * (seq_t)next_k + (seq_t)7;
            seq_t l9 = (seq_t)10 * (seq_t)next_k + (seq_t)9;

            /* All other numbers in the decade must be composite. */
            if (!isPrimeGeneric(l1) && !isPrimeGeneric(l7) && !isPrimeGeneric(l9)) {
                /* Grow cache if needed. */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();              /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
                ++next_k;   /* advance k for the next search */
                break;      /* found the required prime */
            }
            ++next_k;      /* try next decade */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Undefined for n < 1 – return 0 as a safe placeholder. */
        return (seq_t)0;
    }
    ensure_cache(n);
    return cache[n - (idx_t)1];
}
