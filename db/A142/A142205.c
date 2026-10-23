/*
 * A142205  Primes congruent to 8 mod 41.
 * https://oeis.org/A142205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 8 mod 41
 * Range:     u8 n <= 1, u16 n <= 160, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the n‑th prime p such that p ≡ 8 (mod 41) */
/* Uses a dynamic sieve that is rebuilt only when a larger limit is needed */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 by the OEIS offset.
       Return the (n‑1)‑based element of a dynamically filled table.  */
    static uint64_t *all_primes   = NULL;   /* all primes ≤ current limit */
    static size_t   all_cnt      = 0;
    static uint64_t *mod_primes   = NULL;   /* primes ≡ 8 (mod 41) */
    static size_t   mod_cnt      = 0;
    static uint64_t built_limit  = 0;       /* highest limit processed */

    /* Fast path – already have the answer. */
    if (mod_cnt >= (size_t)n)
        return (seq_t)mod_primes[(size_t)n - 1];

    /* Determine a new limit.  Start with a small bound if nothing built yet. */
    uint64_t limit = (built_limit == 0) ? 200 : built_limit * 2 + 100;

    /* Build sieve until we have at least n qualifying primes. */
    while (true) {
        /* Rebuild if the previous limit is too small. */
        if (built_limit < limit) {
            /* Free any previous allocations. */
            if (all_primes) { free(all_primes); all_primes = NULL; all_cnt = 0; }
            if (mod_primes) { free(mod_primes); mod_primes = NULL; mod_cnt = 0; }

            /* Generate all primes up to the new limit. */
            size_t tmp_cnt;
            build_primes_upto(limit, &all_primes, &tmp_cnt);
            all_cnt = tmp_cnt;

            /* Extract those congruent to 8 modulo 41. */
            mod_primes = (uint64_t *)malloc(sizeof(uint64_t) * all_cnt);
            mod_cnt = 0;
            for (size_t i = 0; i < all_cnt; ++i) {
                if (all_primes[i] % 41 == 8) {
                    mod_primes[mod_cnt++] = all_primes[i];
                }
            }
            built_limit = limit;
        }

        /* If we now have enough terms, return the requested one. */
        if (mod_cnt >= (size_t)n)
            return (seq_t)mod_primes[(size_t)n - 1];

        /* Otherwise, enlarge the limit and try again. */
        limit = built_limit * 2 + 100;
    }
}
