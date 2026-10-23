/*
 * A142928  Primes congruent to 7 mod 64.
 * https://oeis.org/A142928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p satisfying p ≡ 7 (mod 64)
 * Range:     u8 n <= 3, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a sieve to collect primes up to a dynamic limit until the
   desired count is reached.  The result is memoised so subsequent
   calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t   *primes_mod      = NULL; /* cached primes ≡ 7 (mod 64) */
static size_t  primes_mod_cnt  = 0;     /* number of cached primes   */
static size_t  primes_mod_cap  = 0;     /* allocated capacity        */
static bool    primes_mod_init = false; /* initialisation flag       */

/* Ensure that primes_mod contains at least n elements.  If not,
   extend the sieve limit and rebuild the cache. */
static void
ensure_primes_mod(idx_t n)
{
    if (primes_mod_init && primes_mod_cnt >= (size_t)n)
        return;

    /* initial guess for sieve limit */
    size_t limit = 12000000; /* 12 million covers ~10000 desired primes   */
    while (1) {
        uint64_t *primes = NULL;
        size_t    prime_cnt = 0;

        build_primes_upto(limit, &primes, &prime_cnt);

        /* allocate temporary storage for primes ≡ 7 (mod 64) */
        size_t tmp_cap = prime_cnt; /* worst case */
        seq_t *tmp = NULL;
        if (tmp_cap > 0) {
            tmp = (seq_t *)malloc(tmp_cap * sizeof(seq_t));
        }

        size_t idx = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if ((primes[i] & 63ULL) == 7ULL) { /* faster % 64 */
                tmp[idx++] = (seq_t)primes[i];
            }
        }
        /* free old prime array */
        free(primes);

        /* If we already had a cache, free it */
        if (primes_mod) free(primes_mod);

        primes_mod      = tmp;
        primes_mod_cnt  = idx;
        primes_mod_cap  = tmp_cap;
        primes_mod_init = true;

        /* If we have enough primes, stop */
        if (primes_mod_cnt >= (size_t)n)
            break;

        /* Not enough: increase limit and retry */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if ((idx_t)1 > n) /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    ensure_primes_mod(n);
    return primes_mod[(size_t)n - 1];
}
