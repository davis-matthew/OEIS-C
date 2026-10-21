/*
 * A070183  Primes p such that x^6 = 2 has a solution mod p, but x^(6^2) = 2
 *          has no solution mod p.
 * https://oeis.org/A070183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that 2 is a 6th power residue
 *            modulo p (i.e., ∃x : x^6≡2 (mod p)) but 2 is not a 36th power
 *            residue modulo p (i.e., no x : x^36≡2 (mod p))
 * Range:     u8 n <= 3, u16 n <= 614, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* ------------------------------------------------------------------ */
/* Dynamic array of 64‑bit values (used for primes and terms).       */
typedef struct {
    uint64_t *data;
    size_t   len;
    size_t   cap;
} U64Vec;

static void u64vec_push(U64Vec *v, uint64_t x)
{
    if (v->len == v->cap) {
        size_t newcap = v->cap ? v->cap + v->cap/2 : 16;
        uint64_t *newdata = (uint64_t *)realloc(v->data, newcap * sizeof(uint64_t));
        if (!newdata) abort();           /* out of memory */
        v->data = newdata;
        v->cap  = newcap;
    }
    v->data[v->len++] = x;
}

/* Euclidean gcd for 64‑bit integers. */

/* Modular exponentiation: base^exp mod mod (mod ≤ 2^63). */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* Test whether n is prime by trial division using known primes. */
static bool is_prime_candidate(uint64_t n, const U64Vec *primes)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes->len; ++i) {
        uint64_t p = primes->data[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate the next prime > last and store it in primes. */
static uint64_t next_prime_uint64(uint64_t last, U64Vec *primes)
{
    if (last < 2) {
        u64vec_push(primes, 2);
        return 2;
    }
    uint64_t candidate = last + 1;
    if (candidate % 2 == 0) ++candidate;   /* skip even numbers */
    for (;;) {
        if (is_prime_candidate(candidate, primes)) {
            u64vec_push(primes, candidate);
            return candidate;
        }
        candidate += 2;
    }
}

/* Check the condition for a prime p. */
static bool good_prime(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t g1 = gcd64(6ULL,  phi);
    uint64_t g2 = gcd64(36ULL, phi);
    uint64_t exp1 = phi / g1;
    uint64_t exp2 = phi / g2;
    uint64_t pow1 = mod_pow_uint64(2ULL, exp1, p);
    uint64_t pow2 = mod_pow_uint64(2ULL, exp2, p);
    return (pow1 == 1) && (pow2 != 1);
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is 1‑based */

    /* Static storages retained between calls. */
    static U64Vec primes   = {NULL, 0, 0};
    static U64Vec terms    = {NULL, 0, 0};
    static uint64_t last_checked = 1;

    /* Generate primes and collect good ones until we have n terms. */
    while (terms.len < n) {
        uint64_t p = next_prime_uint64(last_checked, &primes);
        last_checked = p;
        if (good_prime(p))
            u64vec_push(&terms, p);
    }

    return (seq_t)terms.data[n - 1];
}
