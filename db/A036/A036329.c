/*
 * A036329  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 5.
 * https://oeis.org/A036329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 9840, u32 n <= 10077, u64 n <= 10077,
 *            u128 n <= 10077, big any n
 * Validated: full, pass, 10077 terms through n = 10077 (2026-11-03)
 * Helpers:   factor_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A036329: composite numbers whose concatenated prime factors
   have exactly 5 digits.  The sequence is generated on demand and cached
   for fast repeated look‑ups. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <math.h>
#include "../utils/factor_u64.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;   /* array of found terms */
static size_t cache_len = 0;  /* number of terms currently stored   */
static size_t cache_cap = 8;  /* current allocation capacity        */
static uint64_t next_candidate = 2ULL;  /* next integer to test */

/* Ensure the cache can hold at least `need` terms (1‑based). */
static void ensure_cache(size_t need)
{
    /* Grow cache capacity if required. */
    if (cache_len < need) {
        if (cache == NULL) {
            cache_cap = (need > 8) ? need : 8;
            cache = mem_new(seq_t, cache_cap);
            if (!cache)
                exit(EXIT_FAILURE);
        } else if (cache_cap < need) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < need)
                new_cap = need;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Temporary buffers for factorisation. */
        uint64_t primes[32];
        uint64_t exps[32];
        size_t pcnt;

        while (cache_len < need) {
            uint64_t cand = next_candidate;
            /* Factorise the candidate.  For a prime the vector will
               contain a single element with exponent 1. */
            factor_u64(cand, primes, exps, &pcnt);

            /* Skip primes. */
            if (!(pcnt == 1 && exps[0] == 1ULL)) {
                unsigned total_len = 0;
                for (size_t i = 0; i < pcnt; ++i) {
                    unsigned d = num_digits0(primes[i]);      /* digits of the prime */
                    total_len += d * (unsigned)exps[i];       /* each occurrence */
                    if (total_len > 5)                        /* early exit */
                        break;
                }
                if (total_len == 5)                          /* matches requirement */
                    cache[cache_len++] = cand;               /* store it */
            }
            ++next_candidate;
        }
    }
}

/* Public sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n = 0 */
    ensure_cache((size_t)n);
    return cache[n - 1];
}
