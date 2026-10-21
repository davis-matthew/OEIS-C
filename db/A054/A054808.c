/*
 * A054808  First term of strong prime quintets: p(m+1)-p(m) > p(m+2)-p(m+1) >
 *          p(m+3)-p(m+2) > p(m+4)-p(m+3).
 * https://oeis.org/A054808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054808: first prime of each strong prime quintet.
   A strong quintet is a sequence of five consecutive primes
   p₁<p₂<p₃<p₄<p₅ whose successive gaps satisfy
       (p₂−p₁) > (p₃−p₂) > (p₄−p₃) > (p₅−p₄).
   The function returns the first prime p₁ of each such quintet.
   The algorithm generates primes incrementally (trial division),
   keeps the last five gaps and records a(prime) whenever they are
   strictly decreasing. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* helper: primality test by trial division using already-found primes */
static bool is_prime_candidate(uint64_t x, const uint64_t *primes, size_t prime_cnt)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* array size increments */
#define INIT_CAP 64
#define GROW_FACTOR 2

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *a_cache      = NULL;
    static size_t a_count      = 0;
    static size_t a_cap        = 0;

    /* static list of primes for generation */
    static uint64_t *primes    = NULL;
    static size_t prime_cnt    = 0;
    static size_t prime_cap    = 0;
    static uint64_t candidate  = 2; /* next number to test for primality */

    /* grow a_cache if needed */
    if (a_cache == NULL) {
        a_cap = INIT_CAP;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    }

    /* grow primes array if needed */
    if (primes == NULL) {
        prime_cap = INIT_CAP;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* generate primes until we have computed the desired element */
    while (a_count < (size_t)n) {
        uint64_t p;

        /* get next prime */
        while (true) {
            if (is_prime_candidate(candidate, primes, prime_cnt)) {
                p = candidate;
                break;
            }
            candidate += (candidate == 2 ? 1 : 2); /* after 2 try odd numbers */
        }

        /* store new prime */
        if (prime_cnt >= prime_cap) {
            prime_cap *= GROW_FACTOR;
            primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
        }
        primes[prime_cnt++] = p;

        /* check for strong quintet when we have at least 5 primes */
        if (prime_cnt >= 5) {
            uint64_t d1 = primes[prime_cnt-4] - primes[prime_cnt-5];
            uint64_t d2 = primes[prime_cnt-3] - primes[prime_cnt-4];
            uint64_t d3 = primes[prime_cnt-2] - primes[prime_cnt-3];
            uint64_t d4 = primes[prime_cnt-1] - primes[prime_cnt-2];
            if (d1 > d2 && d2 > d3 && d3 > d4) {
                /* store the first prime of the quintet */
                if (a_count >= a_cap) {
                    a_cap *= GROW_FACTOR;
                    a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
                }
                a_cache[a_count++] = (seq_t)primes[prime_cnt-5];
            }
        }

        /* advance candidate to next possible prime */
        candidate = (candidate == 2 ? 3 : candidate + 2);
    }

    return a_cache[n-1];
}
