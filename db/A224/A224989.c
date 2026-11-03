/*
 * A224989  Let p = prime(n). a(n) = number of primes q less than p, such that
 *          both p-q+1 and p-q-1 are primes.
 * https://oeis.org/A224989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 979 (2026-11-01)
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A224989: a(n) = number of primes q < p(n) such that both p-q+1 and
   p-q-1 are primes, where p = prime(n). */

#include <stdint.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/sieve_primes.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* nth prime */
    uint32_t p32 = get_prime((idx_t)n);
    idx_t p = (idx_t)p32;

    /* Build all primes ≤ p. */
    idx_t *primes = NULL;
    size_t prime_cnt;
    sieve_primes(p, &primes, &prime_cnt);

    seq_t count = (seq_t)0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t q = primes[i];
        if (q == p) break;          /* skip the prime p itself */

        idx_t r = p - q + (idx_t)1;
        idx_t s = p - q - (idx_t)1;

        if (is_prime(r) && is_prime(s))
            ++count;
    }

    /* free allocated memory */
    free(primes);

    return count;
}
