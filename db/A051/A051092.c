/*
 * A051092  Primes p such that x^46 = -2 has a solution mod p.
 * https://oeis.org/A051092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, mulmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A051092: primes p such that x^46 ≡ -2 (mod p) has a solution.
 * The sequence consists of all odd primes p with gcd(p‑1,46)=1 and
 * (-2)^((p‑1)/gcd(p‑1,46)) ≡ 1 (mod p) together with the prime 2.
 * For each odd prime p, this condition checks whether −2 is a 46‑th
 * power residue modulo p.
 * We sieve primes up to a fixed limit (8 000 000) and collect the
 * sequence elements lazily.  The array of known terms is grown
 * as needed so calls to a(n) may arrive in any order.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/mulmod_u64.h"

/* ---------- Helper: 64‑bit gcd (doesn't conflict with existing names) ---------- */

/* ---------- Helper: modular multiplication using 128‑bit intermediate ---------- */

/* ---------- Helper: modular exponentiation ---------- */
static inline uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) {
            result = mulmod_u64(result, base, mod);
        }
        base = mulmod_u64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- Prime sieve (single initialisation) ---------- */
#define SIEVE_LIMIT 8000000u   /* large enough for first 10 000 terms */

static bool *prime_sieve = NULL;          /* true = composite */
static uint32_t *prime_list = NULL;       /* list of primes */
static size_t prime_count = 0;            /* total primes in list */
static size_t prime_idx = 0;              /* next prime to examine for sequence */

/* Allocate sieve and populate prime_list */
static void init_prime_sieve(void) {
    if (prime_sieve != NULL) return;  /* already initialised */

    prime_sieve = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!prime_sieve) return;          /* allocation failure – unlikely */

    prime_sieve[0] = prime_sieve[1] = true;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (!prime_sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i) {
                prime_sieve[(size_t)j] = true;
            }
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!prime_sieve[(size_t)i]) ++prime_count;
    }

    /* Allocate list and fill it */
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) return;  /* failure – unlikely */

    size_t pos = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!prime_sieve[(size_t)i]) prime_list[pos++] = i;
    }
}

/* ---------- Sequence storage (lazily filled) ---------- */
static seq_t *seq_terms = NULL;
static size_t seq_len = 0;
static size_t seq_capacity = 0;

/* Grow term array to at least capacity needed */
static void ensure_terms_capacity(size_t needed) {
    if (seq_capacity >= needed) return;
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    seq_capacity = new_cap;
}

/* Process primes until we have at least n terms */
static void fill_terms_to(size_t n) {
    init_prime_sieve();
    ensure_terms_capacity(n);

    while (seq_len < n) {
        if (prime_idx >= prime_count) {
            /* Not enough primes in sieve; can't extend easily. */
            break;
        }
        uint32_t p = prime_list[prime_idx++];
        if (p == 2u) {
            seq_terms[seq_len++] = (seq_t)2u;
            continue;
        }

        uint64_t pm1 = (uint64_t)p - 1;
        uint64_t g = gcd_u64(pm1, 46ULL);
        uint64_t exp = pm1 / g;
        uint64_t base = ((uint64_t)p - 2) % p;           /* -2 mod p */

        if (mod_pow_u64(base, exp, (uint64_t)p) == 1ULL) {
            seq_terms[seq_len++] = (seq_t)p;
        }
    }
}

/* ---------- Public function (exact signature required) ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for index 0 */
    fill_terms_to((size_t)n);
    if (n <= seq_len) return seq_terms[n - 1];
    return (seq_t)0;                   /* safety: should not happen with enough sieve */
}
