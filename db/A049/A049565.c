/*
 * A049565  Primes p such that x^33 = 2 has a solution mod p.
 * https://oeis.org/A049565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _ensure_uint64_cap, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049565: primes p such that the congruence x^33 ≡ 2 (mod p) has a solution.
   The function generates primes on demand and tests the condition with modular
   exponentiation.  It remembers previously found primes so that calls in any
   order are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mod_pow.h"
#include "../utils/_ensure_uint64_cap.h"

/* small Euclidean gcd for uint64_t */
static uint64_t _gcd64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* primality test by trial division using primes already found */
static bool _is_prime(uint64_t n, uint64_t *primes, size_t prime_cnt) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    uint64_t lim = (uint64_t)sqrt((double)n);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > lim) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* return true iff prime p satisfies x^33 ≡ 2 (mod p) */
static bool _prime_satisfies(uint64_t p) {
    if (p == 2) return true;            /* 0^33 == 2 (mod 2) */
    uint64_t g = _gcd64(33ULL, p - 1ULL);          /* gcd(33, p‑1) */
    uint64_t exp = (p - 1ULL) / g;                /* exponent for testing */
    seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);
    return r == (seq_t)1;
}

/* --- public sequence function --------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* undefined, but safe */

    /* static data persist between calls */
    static uint64_t *good_primes = NULL;      /* primes that satisfy */
    static size_t  good_cnt  = 0, good_cap  = 0;

    static uint64_t *prime_list = NULL;       /* all primes generated so far */
    static size_t   prime_cnt  = 0, prime_cap = 0;
    static uint64_t cand = 2;                 /* next candidate to test */

    /* initialise the prime buffer on first entry */
    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* generate terms until we have at least n of them */
    while (good_cnt < (size_t)n) {
        /* find the next prime */
        while (!_is_prime(cand, prime_list, prime_cnt)) {
            cand += (cand == 2) ? 1 : 2;      /* 2, then odd numbers */
        }

        /* store the new prime */
        _ensure_uint64_cap(&prime_list, &prime_cap, prime_cnt + 1);
        prime_list[prime_cnt++] = cand;

        /* test the condition and store if true */
        if (_prime_satisfies(cand)) {
            _ensure_uint64_cap(&good_primes, &good_cap, good_cnt + 1);
            good_primes[good_cnt++] = cand;
        }

        /* advance to next candidate */
        cand += (cand == 2) ? 1 : 2;
    }

    return (seq_t)good_primes[n - 1];
}
