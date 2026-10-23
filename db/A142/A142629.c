/*
 * A142629  Primes congruent to 39 mod 55.
 * https://oeis.org/A142629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 39 mod 55 */
/* The n-th prime p such that p ≡ 39 (mod 55), with a(1) = 149. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Return the n-th prime belonging to the residue class 39 modulo 55. */
seq_t a(idx_t n)
{
    /* offset starts at 1; behaviour for n == 0 is irrelevant. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* static state that persists across calls */
    static idx_t *qual_primes = NULL;   /* array of qualifying primes */
    static size_t  qual_cap   = 0;      /* allocated capacity of qual_primes */
    static size_t  qual_cnt   = 0;      /* number of qualifying primes stored */
    static idx_t   last_limit = 0;      /* last limit used for building primes */
    static size_t  processed  = 0;      /* number of primes already examined */

    /* If we already have enough qualifying primes, return immediately. */
    if (qual_cnt >= (size_t)n)
        return (seq_t)qual_primes[n - 1];

    /* Ensure the array of qualifying primes has sufficient capacity. */
    if (qual_cap < qual_cnt + 1) {
        size_t new_cap = qual_cap == 0 ? 16 : qual_cap * 2;
        while (new_cap < qual_cnt + 1)
            new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(qual_primes, new_cap * sizeof(idx_t));
        if (!tmp) {
            /* Allocation failed; clean up and return 0 (unreasonable case). */
            free(qual_primes);
            qual_primes = NULL;
            qual_cap = 0;
            qual_cnt = 0;
            return (seq_t)0;
        }
        qual_primes = tmp;
        qual_cap = new_cap;
    }

    /* Grow the prime sieve until we have enough qualifying primes. */
    while (qual_cnt < (size_t)n) {
        /* Choose a new limit: start at 100, otherwise double the previous limit. */
        idx_t new_limit = last_limit == 0 ? 100ULL : last_limit * 2ULL;

        /* Build all primes up to new_limit. */
        idx_t *all_primes = NULL;
        size_t all_cnt = 0;
        build_primes(new_limit, &all_primes, &all_cnt);

        /* Process only the new primes that were not examined before. */
        for (size_t i = processed; i < all_cnt; ++i) {
            idx_t p = all_primes[i];
            if (p % (idx_t)55 == (idx_t)39) {
                /* Store the qualifying prime. */
                qual_primes[qual_cnt++] = p;
                /* Ensure enough capacity for the next candidate. */
                if (qual_cnt == qual_cap) {
                    size_t new_cap = qual_cap * 2;
                    idx_t *tmp = (idx_t *)realloc(qual_primes, new_cap * sizeof(idx_t));
                    if (!tmp) {
                        free(all_primes);
                        free(qual_primes);
                        qual_primes = NULL;
                        qual_cap = 0;
                        qual_cnt = 0;
                        return (seq_t)0;
                    }
                    qual_primes = tmp;
                    qual_cap = new_cap;
                }
            }
        }

        /* Prepare for the next iteration. */
        processed = all_cnt;
        last_limit = new_limit;
        free(all_primes);
    }

    /* Return the requested prime, converted to seq_t. */
    return (seq_t)qual_primes[n - 1];
}
