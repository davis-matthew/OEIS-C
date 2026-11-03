/*
 * A069115  Squarefree part of prime(n)+1 : the smallest number such that
 *          a(n)*(prime(n)+1) is a square.
 * https://oeis.org/A069115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = squarefree part of prime(n)+1 (the smallest integer a
 *            such that a*(prime(n)+1) is a square)
 * Range:     u8 n <= 18, u16 n <= 799, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9221 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers from the utils library */
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}


/* Cached list of primes used for factorisation */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t  prime_cache_limit = 0;

/* Return the squarefree part of an integer: product of primes with odd exponent */
static seq_t squarefree_part(seq_t m)
{
    /* quick path: if m is 0 or 1 */
    if (m <= 1)
        return m;

    /* compute integer square root of m to bound the factors */
    seq_t sqrt_m = floor_sqrt(m);
    idx_t bound = (idx_t)sqrt_m;
    if (bound < 2) bound = 2;

    /* resize prime cache if necessary */
    if (bound > prime_cache_limit || prime_cache == NULL) {
        if (prime_cache != NULL) {
            free(prime_cache);
            prime_cache = NULL;
            prime_cache_cnt = 0;
        }
        build_primes_upto(bound, &prime_cache, &prime_cache_cnt);
        prime_cache_limit = bound;
    }

    seq_t result = 1;
    /* trial division using cached primes */
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        seq_t p = (seq_t)prime_cache[i];
        if (p * p > m) break;

        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp & 1)          /* odd exponent contributes to squarefree part */
            result *= p;
    }

    /* If m > 1 now, it is a prime factor with exponent 1 */
    if (m > 1)
        result *= m;

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n=0; make it harmless */

    /* Get the nth prime (1‑based) */
    uint32_t p = get_prime(n);

    /* Compute prime(n)+1 as seq_t */
    seq_t m = (seq_t)p + 1;

    /* Return its squarefree part */
    return squarefree_part(m);
}
