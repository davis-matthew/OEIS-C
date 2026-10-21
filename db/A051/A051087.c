/*
 * A051087  Primes p such that x^36 = -2 has a solution mod p.
 * https://oeis.org/A051087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051087: primes p such that x^36 ≡ -2 (mod p) has a solution.
   The algorithm:  for each prime p, compute d = gcd(36, p-1), e = (p-1)/d.
   Then -2 is a 36‑th power modulo p iff pow_mod(p-2, e, p) == 1.
   All primes are generated once by a sieve up to a fixed limit.
   The sequence terms are cached so that repeated calls stay O(1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- helper routines ---------------------------------------------------- */

/* Euclidean GCD for idx_t. */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Modulo exponentiation for 64‑bit arguments. */
static inline uint64_t modpow_uint64(uint64_t base, idx_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (uint64_t)(((__uint128_t)result * base) % mod);
        base = (uint64_t)(((__uint128_t)base * base) % mod);
        exp >>= 1;
    }
    return result;
}

/* --- sieve of Eratosthenes -------------------------------------------- */

#define SIEVE_LIMIT 30000000UL          /* 30 million is enough for 10k terms */

static bool *sieve_array = NULL;
static idx_t *prime_list = NULL;
static idx_t prime_count = 0;
static idx_t prime_capacity = 0;

/* initialise the sieve and prime list */
static void init_sieve(void) {
    if (sieve_array) return; /* already initialised */

    sieve_array = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!sieve_array) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) sieve_array[i] = true;
    sieve_array[0] = sieve_array[1] = false;

    for (idx_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (sieve_array[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_array[j] = false;

    prime_capacity = 2000000; /* enough for primes up to 30 million */
    prime_list = (idx_t*)malloc(prime_capacity * sizeof(idx_t));
    if (!prime_list) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve_array[i]) {
            if (prime_count == prime_capacity) {
                prime_capacity *= 2;
                prime_list = (idx_t*)realloc(prime_list,
                                             prime_capacity * sizeof(idx_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
            prime_list[prime_count++] = i;
        }
}

/* --- sequence caching ------------------------------------------------- */

static seq_t *term_array = NULL;
static idx_t term_count = 0;
static idx_t term_capacity = 0;
static idx_t next_prime_idx = 0;

static void init_terms(void) {
    if (term_array) return;
    term_capacity = 128;
    term_array = (seq_t*)malloc(term_capacity * sizeof(seq_t));
    if (!term_array) exit(EXIT_FAILURE);
}

/* append a term to the cache */
static void append_term(seq_t val) {
    if (term_count == term_capacity) {
        term_capacity *= 2;
        term_array = (seq_t*)realloc(term_array,
                                     term_capacity * sizeof(seq_t));
        if (!term_array) exit(EXIT_FAILURE);
    }
    term_array[term_count++] = val;
}

/* --- the required function -------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* out of bounds, but never called */
    init_sieve();
    init_terms();

    /* generate terms until we have at least n of them */
    while (term_count < n) {
        if (next_prime_idx >= prime_count) {
            /* this should never happen for the prescribed limits */
            break;
        }
        idx_t p = prime_list[next_prime_idx++];
        if (p == 2) {                    /* 2 always satisfies the equation */
            append_term((seq_t)p);
            continue;
        }
        idx_t d = gcd_idx((idx_t)36, p - 1);
        idx_t exp = (p - 1) / d;
        uint64_t base = (p >= 2) ? (p - 2) : (0);
        if (modpow_uint64(base, exp, p) == 1) {
            append_term((seq_t)p);
        }
    }
    return term_array[n - 1];
}
