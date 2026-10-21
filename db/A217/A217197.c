/*
 * A217197  Primes p such that p-3 is the greatest semiprime less than p.
 * https://oeis.org/A217197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 667, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: find primes p such that p‑3 is the greatest semiprime
   less than p.  The code builds the list of primes incrementally via
   trial division, and stores every qualifying prime in a cache.
   The routine is memoised so that calls to a(n) in any order are fast.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_floor_isqrt_u64.h"

static uint64_t *primes   = NULL;   /* array of discovered primes   */
static size_t   primes_n = 0;       /* number of primes             */
static size_t   primes_cap = 0;     /* allocated capacity          */

static uint64_t *terms   = NULL;   /* sequence terms (p satisfying rule) */
static size_t   terms_n = 0;       /* number of terms stored        */
static size_t   terms_cap = 0;

static uint64_t cur = 4;            /* next candidate to test        */

/* Ensure the primes array has enough space and add a new prime. */
static void add_prime(uint64_t p)
{
    if (primes_n == primes_cap) {
        size_t newcap = primes_cap ? primes_cap * 2 : 128;
        uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = newcap;
    }
    primes[primes_n++] = p;
}

/* Ensure the terms array has enough space and add a new sequence value. */
static void add_term(uint64_t v)
{
    if (terms_n == terms_cap) {
        size_t newcap = terms_cap ? terms_cap * 2 : 128;
        uint64_t *tmp = (uint64_t *)realloc(terms, newcap * sizeof(uint64_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = newcap;
    }
    terms[terms_n++] = v;
}

/* Test whether n is prime using the known primes list. */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t r = _floor_isqrt_u64(n);
    for (size_t i = 0; i < primes_n && primes[i] <= r; ++i) {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

/* Test whether x is a semiprime (product of exactly two primes). */
static bool is_semiprime(uint64_t x)
{
    if (x < 2) return false;
    uint64_t r = _floor_isqrt_u64(x);
    for (size_t i = 0; i < primes_n && primes[i] <= r; ++i) {
        uint64_t p = primes[i];
        if (x % p == 0) {
            uint64_t q = x / p;
            uint64_t r2 = _floor_isqrt_u64(q);
            bool qprime = true;
            for (size_t j = 0; j < primes_n && primes[j] <= r2; ++j) {
                if (q % primes[j] == 0) { qprime = false; break; }
            }
            return qprime;
        }
    }
    return false;
}

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* harmless for n<1 */
    /* initialise primes list with 2 and 3 if empty */
    if (primes_n == 0) { add_prime(2); add_prime(3); }
    /* generate terms until we have n terms */
    while (terms_n < (size_t)n) {
        /* find next prime */
        while ((size_t)cur < UINT64_MAX && !is_prime(cur)) ++cur;
        if ((size_t)cur == UINT64_MAX) break; /* safety */
        uint64_t p = cur;
        add_prime(p);                          /* store prime for future use */
        /* check semiprime condition */
        if (p >= 3 &&
            is_semiprime(p - 3) &&
            !is_semiprime(p - 2) &&
            !is_semiprime(p - 1)) {
            add_term(p);
        }
        ++cur;
    }
    return (seq_t)terms[n - 1];
}
