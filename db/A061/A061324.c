/*
 * A061324  Primes with 11 as smallest positive primitive root.
 * https://oeis.org/A061324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 237, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061324: primes for which 11 is the smallest positive primitive root.
   Implementation: linear scan of primes until the n‑th one satisfying the
   property.  Uses a simple sieve of Eratosthenes and elementary
   modular exponentiation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*----- modular exponentiation (unsigned __int128 for safety) -----*/
static uint64_t powmod_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1ULL) r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1ULL;
    }
    return r;
}

/*----- recommend a sieve bound for the n‑th required prime -----*/
static uint64_t estimate_limit(idx_t n)
{
    if (n < 6) return 100ULL;
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn)));
    return (uint64_t)(est) + 10ULL;
}

/*----- test whether 11 is the smallest primitive root modulo p -----*/
static bool is_11_smallest_primitive(uint64_t p,
                                     const uint64_t *primes,
                                     size_t primes_cnt)
{
    if (p <= 11ULL) return false;

    /* Factorise p-1 to obtain distinct prime divisors. */
    uint64_t factors[64];
    size_t fcnt = 0;
    uint64_t m = p - 1ULL;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t q = primes[i];
        if (q > m / q) break;   /* q*q > m */
        if (m % q == 0ULL) {
            factors[fcnt++] = q;
            while (m % q == 0ULL) m /= q;
        }
    }
    if (m > 1ULL) factors[fcnt++] = m;

    /* 11 must have full order. */
    for (size_t i = 0; i < fcnt; ++i) {
        if (powmod_u64(11ULL, (p - 1ULL) / factors[i], p) == 1ULL)
            return false;
    }

    /* No integer a in [2,10] may be a primitive root. */
    for (int a = 2; a < 11; ++a) {
        bool ok = true;
        for (size_t i = 0; i < fcnt; ++i) {
            if (powmod_u64((uint64_t)a, (p - 1ULL) / factors[i], p) == 1ULL) {
                ok = false;
                break;
            }
        }
        if (ok) return false;          /* a is a primitive root → reject */
    }
    return true;
}

/*----- sequence function -----*/
seq_t a(idx_t n)
{
    if (n == 0U) return 0;           /* undefined for indices < 1 */

    /* Allocate a reasonable upper bound and expand if necessary. */
    uint64_t limit = estimate_limit(n) * 20ULL;  /* safety factor */
    if (limit < 100ULL) limit = 100ULL;

    while (true) {
        /* Sieve of Eratosthenes: memory for composite flags. */
        uint8_t *is_comp = (uint8_t *)calloc(limit + 1U, sizeof(uint8_t));
        if (!is_comp) abort();          /* out of memory */

        /* Array to store primes discovered. */
        size_t primes_cap = limit / 10U + 10U;
        uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) { free(is_comp); abort(); }

        size_t primes_cnt = 0;

        /* Build prime list. */
        for (uint64_t i = 2ULL; i <= limit; ++i) {
            if (!is_comp[(size_t)i]) {
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1U;
                    uint64_t *tmp = (uint64_t *)realloc(
                        primes, primes_cap * sizeof(uint64_t));
                    if (!tmp) { free(is_comp); free(primes); abort(); }
                    primes = tmp;
                }
                primes[primes_cnt++] = i;

                if (i * i <= limit) {
                    for (uint64_t j = i * i; j <= limit; j += i)
                        is_comp[(size_t)j] = 1U;
                }
            }
        }

        /* Scan for the required property. */
        size_t found = 0;
        for (size_t idx = 0; idx < primes_cnt; ++idx) {
            uint64_t p = primes[idx];
            if (is_11_smallest_primitive(p, primes, primes_cnt)) {
                ++found;
                if (found == n) {
                    free(is_comp);
                    free(primes);
                    return (seq_t)p;
                }
            }
        }

        /* Not enough terms found: expand the search space. */
        free(is_comp);
        free(primes);
        limit <<= 1U;             /* double the limit */
    }
}
