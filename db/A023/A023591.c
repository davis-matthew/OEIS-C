/*
 * A023591  Greatest exponent in prime-power factorization of 2*prime(n)+1.
 * https://oeis.org/A023591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest exponent in the prime‑power factorization of
 *            2*prime(n)+1
 * Range:     u8 n <= 32, u16 n <= 3718, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9560 (2026-11-02)
 * Helpers:   build_primes_upto, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Provided utilities */
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"

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

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety */
    if (n < 1) return (seq_t)0;

    /* Prime number of the given index (1 – based). */
    uint32_t p = get_prime(n);

    /* Compute 2*p + 1 as seq_t, using the recommended cast order. */
    seq_t m = (seq_t)2 * (seq_t)p + (seq_t)1;

    /* Determine the upper bound for trial division. */
    idx_t limit = isqrt((idx_t)m);
    if (limit < 2) limit = 2;

    /* Generate all primes up to sqrt(m). */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Factor m and find the largest exponent. */
    idx_t temp = (idx_t)m;          /* temporary value for division */
    seq_t max_exp = (seq_t)0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t prime_i = (idx_t)primes[i];
        if (prime_i * prime_i > temp) break;   /* small optimisation */

        if (temp % prime_i == 0) {
            unsigned cnt = 0;
            while (temp % prime_i == 0) {
                temp /= prime_i;
                ++cnt;
            }
            if (cnt > (unsigned)max_exp) max_exp = (seq_t)cnt;
        }
    }

    /* If after trial division a factor > 1 remains, it is a prime
       factor with exponent 1. */
    if (temp > 1) {
        if (1 > (unsigned)max_exp) max_exp = (seq_t)1;
    }

    /* Clean up */
    free(primes);

    return max_exp;
}
