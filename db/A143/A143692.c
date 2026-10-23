/*
 * A143692  Permutation of natural numbers: If n is k-th number with an odd
 *          number of prime divisors (counted with multiplicity) [i.e., n =
 *          A026424(k)], a(n) = 2*k, otherwise, when n is k-th number with an
 *          even number of prime divisors [i.e., n = A028260(k)], a(n) =
 *          (2*k)-1.
 * https://oeis.org/A143692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For each natural number n ≥ 1 * let ω(n) be the big omega
 *            (number of prime factors with multiplicity) * Let O(n) be the
 *            number of integers ≤ n with odd ω * If n has odd ω, a(n) =
 *            2·O(n); otherwise a(n) = 2·(n – O(n)) – 1. * * The routine
 *            builds at most one table of small‑omega values on first use...
 * Range:     u8 n <= 245, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build. */

static size_t   max_n      = 0;        /* largest n for which tables are built */
static uint8_t *bigomega   = NULL;    /* ω(i) for i ≤ max_n   (uint8_t is enough) */
static uint32_t *oddpref   = NULL;    /* prefix count of odd‑ω numbers   */
static uint32_t *spf       = NULL;    /* smallest prime factor of i       */
static uint32_t *primes    = NULL;    /* list of primes */
static size_t   primes_cnt = 0;       /* number of primes found */
static size_t   primes_cap = 0;       /* current capacity of primes array */

/* Ensure tables are valid up to at least n */
static void extend_upto(idx_t n)
{
    if (n <= (idx_t)max_n) return;           /* already sufficient */

    size_t newmax = (size_t)n;
    if (newmax < 2u) newmax = 2u;            /* at least indices 0,1,2 */

    /* Grow bigomega, oddpref, spf arrays */
    bigomega = (uint8_t *)realloc(bigomega, sizeof(uint8_t)  * (newmax + 1));
    oddpref  = (uint32_t *)realloc(oddpref,  sizeof(uint32_t) * (newmax + 1));
    spf      = (uint32_t *)realloc(spf,      sizeof(uint32_t) * (newmax + 1));
    if (!bigomega || !oddpref || !spf) abort();

    /* Grow primes array if needed */
    if (primes == NULL || primes_cap < (newmax + 1u)) {
        size_t newcap = newmax + 1u;
        uint32_t *newprimes = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
        if (!newprimes) abort();
        primes = newprimes;
        primes_cap = newcap;
    }

    /* Initialise spf for newly added indices */
    for (size_t i = max_n + 1u; i <= newmax; ++i) spf[i] = 0;

    /* Linear sieve to compute spf and primes up to newmax */
    for (size_t i = 2u; i <= newmax; ++i) {
        if (spf[i] == 0u) {
            spf[i] = (uint32_t)i;
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0u; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || i * (uint64_t)p > (uint64_t)newmax) break;
            spf[i * p] = p;
        }
    }

    /* Initialize base cases (only when first called) */
    if (max_n == 0u) {
        bigomega[0] = 0u;
        bigomega[1] = 0u;              /* ω(1) = 0 (even) */
        oddpref[0]  = 0u;
        oddpref[1]  = 0u;
    }

    /* Compute bigomega and oddpref for new indices */
    size_t start = (max_n < 2u ? 2u : max_n + 1u);
    for (size_t i = start; i <= newmax; ++i) {
        uint32_t p = spf[i];
        uint32_t m = i / p;
        bigomega[i] = bigomega[m] + 1u;
        oddpref[i]  = oddpref[i - 1u] + (bigomega[i] & 1u);
    }

    max_n = newmax;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;           /* sequence defined for n ≥ 1 */

    extend_upto(n);

    uint8_t omega = bigomega[(size_t)n];
    if (omega & 1u) {                       /* odd big‑omega */
        uint32_t k = oddpref[(size_t)n];
        return (seq_t)((seq_t)k * 2u);
    } else {                                /* even big‑omega */
        uint32_t k = (uint32_t)n - oddpref[(size_t)n];
        return (seq_t)((seq_t)k * 2u - 1u);
    }
}
