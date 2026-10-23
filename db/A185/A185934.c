/*
 * A185934  Lesser of two consecutive primes which both equal 1 (mod 3).
 * https://oeis.org/A185934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A185934: primes p such that p≡1 (mod 3) and the next prime is also ≡1 (mod 3) */
/* Written to compute a(n) on demand, memoising found values for any call order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Returns the n-th term (n ≥ 1) of A185934. */
seq_t a(idx_t n)
{
    /* Static state for memoisation across calls */
    static seq_t *good = NULL;      /* array of good primes found so far */
    static size_t good_cap = 0;     /* capacity of the array            */
    static idx_t  good_len = 0;     /* number of stored good primes     */
    static uint64_t next_candidate = 2; /* next integer to test for primality */
    static uint64_t last_prime = 0;        /* last prime seen                  */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        good = NULL;
        good_cap = 0;
        good_len = 0;
        next_candidate = 2;
        last_prime = 0;
    }

    /* Generate good primes until we have at least n of them */
    while (good_len < n) {
        /* Find the next prime p >= next_candidate */
        uint64_t p;
        for (p = next_candidate; ; ++p) {
            if (isPrimeGeneric((seq_t)p))
                break;
        }

        /* If p ≡ 1 (mod 3) then check the next prime q */
        if (p % 3ULL == 1ULL) {
            /* Find q, the next prime after p */
            uint64_t q_candidate;
            uint64_t q;
            for (q_candidate = p + 1; ; ++q_candidate) {
                if (isPrimeGeneric((seq_t)q_candidate)) {
                    q = q_candidate;
                    break;
                }
            }

            /* If q ≡ 1 (mod 3) store p as a good prime */
            if (q % 3ULL == 1ULL) {
                if (good_len == good_cap) {        /* grow array if needed */
                    size_t new_cap = good_cap == 0 ? 16 : good_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(good, new_cap * sizeof(seq_t));
                    if (!tmp)                /* allocation failure – abort */
                        return 0;
                    good = tmp;
                    good_cap = new_cap;
                }
                good[good_len++] = (seq_t)p;
            }
        }

        /* Prepare for next search */
        next_candidate = p + 1;     /* next integer to test */
        last_prime = p;             /* remember the prime just processed */
    }

    /* Return the n‑th good prime (indices are 1‑based). */
    return good[n - 1];
}
