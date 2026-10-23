/*
 * A019395  Primes with primitive root 72.
 * https://oeis.org/A019395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A019395 – primes that have 72 as a primitive root.
   Each call a(n) returns the n‑th such prime.  The routine
   incrementally searches primes and memoises the results
   in a static uint64_t array; the values are cast to seq_t
   on return.  This works for both integer seq_t types
   and for the arbitrary–precision seq_t class.
*/
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"
#include "../utils/factor_u64.h"

static uint64_t *cached_primes = NULL;
static size_t cached_len = 0;
static size_t cached_cap = 0;

/* Ensure the cache can store at least needed elements.  */
static void ensure_capacity(size_t needed) {
    if (cached_cap >= needed) return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(cached_primes, new_cap * sizeof(uint64_t));
    if (!tmp && new_cap > 0) { /* realloc failed – abort */
        abort();
    }
    cached_primes = tmp;
    cached_cap = new_cap;
}

/* Test whether p is prime (p > 2).  */
static inline bool is_prime_uint64(uint64_t p) {
    return is_prime(p);
}

/* Test whether 72 is a primitive root modulo the odd prime p.  */
static bool is_primitive_root72(uint64_t p) {
    /* 72 ≡ 0 (mod p) cannot be a primitive root.  */
    if (p <= 2 || 72 % p == 0) return false;

    uint64_t base = 72 % p;
    uint64_t phi = p - 1;                /* φ(p) for prime p */
    uint64_t factors[20];
    uint64_t exps[20];
    size_t fcnt = 0;

    factor_u64(phi, factors, exps, &fcnt); /* factorise φ(p) */

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t q = factors[i];
        if (powmod64(base, phi / q, p) == 1) {
            return false;               /* a nontrivial root of unity exists */
        }
    }
    return true;                         /* all tests passed */
}

/* Return the n‑th prime that has 72 as a primitive root.  */
seq_t a(idx_t n) {
    /* n is 1‑based; return cached value if already computed. */
    while (cached_len < n) {
        static uint64_t cand = 3; /* start search from 5 */
        do {
            cand += 2;                /* only odd candidates */
        } while (!is_prime_uint64(cand));

        if (is_primitive_root72(cand)) {
            ensure_capacity(cached_len + 1);
            cached_primes[cached_len++] = cand;
        }
    }
    return (seq_t)cached_primes[n - 1];
}
