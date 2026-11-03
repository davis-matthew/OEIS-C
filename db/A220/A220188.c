/*
 * A220188  Number of representations of n-th prime as 2p + 3q (p, q primes
 *          not necessarily distinct).
 * https://oeis.org/A220188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of the n-th prime as 2p + 3q
 *            with p and q primes (not necessarily distinct). The function
 *            builds the list of all primes up to the n-th prime and counts
 *            all pairs (p,q) that satisfy 2p+3q = nth_prime
 * Range:     u8 n <= 4390, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9798 (2026-11-01)
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

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

seq_t a(idx_t n) {
    /* Get the n‑th prime (1‑indexed) */
    uint32_t nth_prime = get_prime(n);

    /* Generate all primes ≤ nth_prime */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)nth_prime, &primes, &prime_cnt);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > nth_prime / 2) break;
        uint64_t residual = (uint64_t)nth_prime - 2ULL * p;
        if (residual % 3ULL != 0ULL) continue;
        uint64_t q = residual / 3ULL;
        if (q < 2ULL) continue;
        if (is_prime((idx_t)q)) {
            ++count;
        }
    }

    free(primes);
    return count;
}
