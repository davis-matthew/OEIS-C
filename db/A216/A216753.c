/*
 * A216753  Primes p such that x^44 = -2 has no solution mod p.
 * https://oeis.org/A216753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216753 – primes p for which the congruence x^44 = -2 (mod p) has no
 * solution.  Computed by testing primes in order and storing those that violate
 * the criterion (-2)^((p-1)/gcd(44,p-1)) ≡ 1 (mod p).  Results are memoised,
 * so the order of calls is irrelevant. */
#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool */

#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */
#include "../utils/powmod64.h"    /* uint64_t powmod64(uint64_t, uint64_t, uint64_t); */

static seq_t *prime_cache = NULL;    /* 0‑based array holding qualifying primes */
static size_t cache_size = 0;        /* number of stored elements */
static size_t cache_cap  = 0;        /* allocated capacity */
static uint64_t next_prime_to_test = 3; /* odd integer being tested */

/* Euclidean algorithm for uint64_t */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Grow the cache to accommodate at least `need` elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used but safety */
    /* Expand cache until we have at least n elements. */
    while (cache_size < (size_t)n) {
        /* Test current candidate for primality. */
        if (is_prime_big((seq_t)next_prime_to_test)) {
            uint64_t p = next_prime_to_test;
            /* Compute g = gcd(44, p-1) and exponent e = (p-1)/g. */
            uint64_t g = gcd64(44ULL, p - 1ULL);
            uint64_t e = (p - 1ULL) / g;
            /* Test whether (-2)^e ≡ 1 (mod p). */
            uint64_t test = powmod64((p - 2ULL), e, p);
            if (test != 1ULL) {
                /* No solution exists; store this prime. */
                ensure_cache_cap(cache_size + 1);
                prime_cache[cache_size++] = (seq_t)p;
            }
        }
        /* Advance to next odd candidate. */
        next_prime_to_test += 2ULL;
        /* Handle wrap‑around for 64‑bit overflow in the unlikely event it
         * occurs during the targeted n range. */
        if (next_prime_to_test < 3ULL) next_prime_to_test = 3;
    }
    return prime_cache[(size_t)n - 1];
}
