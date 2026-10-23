/*
 * A038930  Primes p such that 51 is not a square mod p.
 * https://oeis.org/A038930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_isqrt_u64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038930: primes p such that 51 is not a quadratic residue modulo p. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/powmod64.h"         /* uint64_t powmod64(uint64_t a, uint64_t exp, uint64_t mod) */
#include "../utils/floor_isqrt_u64.h"   /* idx_t floor_isqrt_u64(idx_t n) */

/* Memoised sequence storage */
static seq_t *terms   = NULL;   /* cached terms */
static size_t terms_cap = 0;     /* capacity of terms array */
static size_t terms_len = 0;     /* number of stored terms */

/* Cache of primes discovered so far */
static uint64_t *prime_list = NULL;  /* primes */
static size_t prime_cap = 0;          /* capacity of prime_list */
static size_t prime_len = 0;          /* number of primes */

/* Next integer to test for primality */
static uint64_t next_candidate = 2;

/* Ensure the terms array has room for at least `need` elements */
static void ensure_terms_capacity(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure the prime list has room for at least `need` elements */
static void ensure_prime_capacity(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Simple deterministic primality test using trial division by known primes */
static int is_prime(uint64_t n)
{
    if (n < 2) return 0;
    idx_t sq = floor_isqrt_u64(n);
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > sq) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* Return the next prime, storing it in prime_list */
static uint64_t next_prime(void)
{
    if (next_candidate == 2) {
        next_candidate = 3;
        ensure_prime_capacity(prime_len + 1);
        prime_list[prime_len++] = 2;
        return 2;
    }

    for (;;) {
        uint64_t cand = next_candidate;
        next_candidate += 2;   /* only odd candidates after 2 */

        if (is_prime(cand)) {
            ensure_prime_capacity(prime_len + 1);
            prime_list[prime_len++] = cand;
            return cand;
        }
    }
}

/* Main sequence function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Generate terms until we have at least n terms */
    while (terms_len < (size_t)n) {
        uint64_t p = next_prime();

        /* First term is always 2 */
        if (p == 2) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)2;
            continue;
        }

        /* Skip primes dividing 51 (3 and 17) – 51 is a square mod them */
        if (p == 3 || p == 17) continue;

        /* Legendre symbol (51|p) via Euler's criterion */
        uint64_t leg = powmod64(51ULL % p, (p - 1) / 2, p);
        if (leg == p - 1) {           /* Legendre symbol = -1 → non‑residue */
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)p;
        }
    }

    return terms[(size_t)n - 1];
}
