/*
 * A062675  Every divisor (except 1) contains the digit 7.
 * https://oeis.org/A062675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 3373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_digit_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for the n‑th integer whose every divisor
   (except 1) contains the decimal digit 7.  The search proceeds
   incrementally, storing already found terms in a static cache. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_digit_7.h"

/* ---------- helpers ---------- */

/* true if every divisor of m (except 1) contains digit 7.
   Uses the smallest prime factor array spf (size LIMIT+1).   */
static bool good(uint64_t m, const uint32_t *spf, size_t limit)
{
    if (m <= 1)
        return false;

    /* factor m into primes and exponents */
    uint64_t primes[20];
    uint8_t  exps[20];
    size_t   k = 0;
    uint64_t tmp = m;

    while (tmp > 1) {
        uint32_t p = spf[tmp];
        uint8_t  e  = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (!has_digit_7(p))          /* a prime lacking 7 makes a divisor fail */
            return false;
        primes[k] = p;
        exps[k]   = e;
        ++k;
    }

    /* enumerate all divisors via a rolling list */
    uint64_t divs[1 << 20];    /* enough for numbers up to our limit                */
    size_t   dlen = 1;
    divs[0] = 1ULL;

    for (size_t i = 0; i < k; ++i) {
        uint64_t p     = primes[i];
        uint8_t  emax  = exps[i];
        size_t   start = dlen;

        uint64_t pow_p = 1ULL;
        for (uint8_t e = 1; e <= emax; ++e) {
            pow_p *= p;
            for (size_t j = 0; j < start; ++j) {
                divs[dlen++] = divs[j] * pow_p;
            }
        }
    }

    /* check all divisors except 1 */
    for (size_t i = 1; i < dlen; ++i) {
        if (!has_digit_7(divs[i]))
            return false;
    }
    return true;
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n == 0 */

    /* static cache of found terms */
    static seq_t *cache  = NULL;
    static size_t cache_size = 0;         /* number of terms stored */

    /* static sieve data (spf up to LIMIT) */
    enum { LIMIT = 10000000U };           /* should be large enough for n <= 10 000 */
    static uint32_t *spf = NULL;
    static bool spf_ready = false;

    /* static current scanning candidate */
    static uint64_t candidate = 2ULL;

    /* initialise spf if not already */
    if (!spf_ready) {
        spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        for (uint32_t i = 0; i <= LIMIT; ++i)
            spf[i] = 0;
        for (uint32_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                if ((uint64_t)i * i <= LIMIT) {
                    for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                        if (spf[(uint32_t)j] == 0)
                            spf[(uint32_t)j] = i;
                }
            }
        }
        spf_ready = true;
    }

    /* grow cache to accommodate n terms */
    while (cache_size < (size_t)n) {
        /* find next number satisfying the property */
        while (candidate <= LIMIT && !good(candidate, spf, LIMIT))
            ++candidate;

        /* candidate must be within our limit */
        if (candidate > LIMIT) {
            /* unlikely for n <= 10 000 with LIMIT = 10 million */
            abort();
        }

        /* add candidate to cache */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (cache_size + 1) * sizeof(seq_t));
        if (!tmp) {
            abort();
        }
        cache = tmp;
        cache[cache_size++] = (seq_t)candidate;
        ++candidate;
    }

    return cache[n - 1];
}
