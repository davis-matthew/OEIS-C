/*
 * A119313  Numbers with a prime as third-smallest divisor.
 * https://oeis.org/A119313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A119313 – Numbers with a prime as third-smallest divisor.
   The implementation scans integers, keeping a cache of generated terms.
   A number m qualifies iff it has at least two distinct prime factors, and
   – the exponent of the smallest prime factor is 1, or
   – the second smallest prime factor p₂ satisfies p₂ ≤ p₁².
   Since p₂ is always prime, this condition is equivalent to “the
   third divisor >1 is prime”.
   The cache allows random-access queries with amortised O(1) time per
   required term. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *term_cache = NULL;      /* stored sequence values          */
static size_t cache_len = 0;          /* number of terms in cache       */
static size_t cache_cap = 0;          /* current capacity of term_cache  */

static uint64_t *prime_list = NULL;   /* list of primes up to current    */
static size_t prime_cnt = 0;          /* number of primes                */
static size_t prime_cap = 0;          /* capacity of prime_list          */

static uint64_t current_candidate = 2;/* next integer to test            */

static inline void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static inline void grow_primes(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    prime_cap = new_cap;
}

/*  Determine whether n satisfies the condition for A119313.  */
static bool qualifies(uint64_t n)
{
    if (n < 6) return false;         /* smallest term is 6          */

    uint64_t rem = n;
    uint64_t p1 = 0, p2 = 0;         /* first and second prime factors   */
    size_t exp1 = 0;                 /* exponent of p1             */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            if (p1 == 0) {
                p1 = p;
                while (rem % p == 0) {
                    rem /= p;
                    ++exp1;
                }
            } else if (p2 == 0) {
                p2 = p;
                while (rem % p == 0) rem /= p;
            }
        }
    }
    if (rem > 1) {
        if (p1 == 0) /* n is prime, never qualifies */
            return false;
        if (p2 == 0) /* no second distinct prime */
            return false;
        p2 = rem;   /* rem is the last prime factor */
    }

    if (p2 == 0) return false;          /* only one distinct prime */

    if (exp1 == 1) return true;          /* smallest exponent 1 → always true */

    uint64_t sq = p1 * p1;               /* p1² fits in 64‑bit for all cases */
    return p2 <= sq;
}

/*  Add a newly found prime to the prime list.  */
static void add_prime(uint64_t p)
{
    grow_primes(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/*  Build the sequence until it contains at least 'target' terms.  */
static void build_up_to(size_t target)
{
    while (cache_len < target) {
        /* factor current_candidate to determine if prime and if it qualifies */
        uint64_t n = current_candidate;
        uint64_t rem = n;
        uint64_t p1 = 0, p2 = 0;
        size_t exp1 = 0;
        bool is_prime = true;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > rem) break;
            if (rem % p == 0) {
                is_prime = false;
                if (p1 == 0) {
                    p1 = p;
                    while (rem % p == 0) {
                        rem /= p;
                        ++exp1;
                    }
                } else if (p2 == 0) {
                    p2 = p;
                    while (rem % p == 0) rem /= p;
                }
            }
        }
        if (rem > 1) {
            if (p1 == 0) { /* n itself is prime */
                /* nothing to do */
            } else {
                is_prime = false;
                if (p2 == 0) p2 = rem;
            }
        }

        if (is_prime) add_prime(n);

        /* check sequence criterion */
        bool ok = false;
        if (!is_prime) {
            if (p2 != 0) {
                if (exp1 == 1) ok = true;
                else {
                    uint64_t sq = p1 * p1;
                    ok = (p2 <= sq);
                }
            }
        }

        if (ok) {
            grow_cache(cache_len + 1);
            term_cache[cache_len++] = (seq_t)n;
        }
        ++current_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */
    build_up_to((size_t)n);
    return term_cache[n - 1];
}
