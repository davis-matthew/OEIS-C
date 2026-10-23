/*
 * A048169  n is divisible by the cube of the number of unitary divisors of n
 *          (A034444).
 * https://oeis.org/A048169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 491, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A048169: a(n) = the n-th positive integer such that n is divisible by
   (number of unitary divisors of n)^3.
   For n with prime factorization n = ∏ p_i^{e_i},
   the number of unitary divisors is 2^{ω(n)} where ω(n) is the
   number of distinct primes.  Thus the condition is
   2^{3 ω(n)} | n, i.e. v₂(n) ≥ 3 ω(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Helper: integer square root of an unsigned 64‑bit value.              */
/* Uses the provided utility function if available, otherwise a fast    */
/* binary search implementation.                                         */
/* --------------------------------------------------------------------- */

static uint64_t isqrt_uint64(uint64_t n) {
#ifdef _floor_isqrt_u64
    return _floor_isqrt_u64(n);
#else
    uint64_t x = n, y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
#endif
}

/* --------------------------------------------------------------------- */
/* Helper: dynamic array of primes.                                     */
/* --------------------------------------------------------------------- */

static uint32_t *primes = NULL;          /* prime list       */
static size_t primes_cnt = 0;            /* number of primes */
static size_t primes_cap = 0;            /* allocated size   */
static uint64_t prime_limit = 1;         /* highest number
                                            we have tested for prime */

static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= prime_limit)
        return;

    /* Allocate more space for the prime list if necessary. */
    if (primes_cap < primes_cnt + 1024) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 1024;
        void *tmp = realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) return;               /* allocation error, ignore */
        primes = (uint32_t *)tmp;
        primes_cap = new_cap;
    }

    for (uint64_t x = prime_limit + 1; x <= limit; ++x) {
        bool isprime = true;
        uint64_t r = isqrt_uint64(x);
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if (p > r) break;
            if (x % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            primes[primes_cnt++] = (uint32_t)x;
        }
    }
    prime_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n) {
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;

    /* Keep generating numbers until we have the nth term. */
    static uint64_t candidate = 0;        /* last checked integer */

    while (cache_sz < (size_t)n) {
        ++candidate;

        /* Factor candidate to compute ω(candidate). */
        uint64_t tmp = candidate;
        uint32_t omega = 0;

        /* Ensure we have enough primes to factor tmp. */
        ensure_primes_up_to(isqrt_uint64(tmp));

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            uint64_t pp = (uint64_t)p * (uint64_t)p;
            if (pp > tmp) break;
            if (tmp % p == 0) {
                ++omega;
                while (tmp % p == 0)
                    tmp /= p;
            }
        }
        if (tmp > 1)
            ++omega;

        /* Condition: v₂(candidate) ≥ 3·ω(candidate).  v₂(candidate) =
           the number of trailing zero bits. */
        bool good;
        if (candidate == 1)
            good = true;
        else
            good = (__builtin_ctzll(candidate) >= 3 * omega);

        if (good) {
            /* Resize cache if needed. */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                void *tmpc = realloc(cache, new_cap * sizeof(seq_t));
                if (!tmpc) return 0;           /* allocation failure */
                cache = (seq_t *)tmpc;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)candidate;
        }
    }

    return cache[(size_t)n - 1];
}
