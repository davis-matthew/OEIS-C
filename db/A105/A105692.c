/*
 * A105692  Minimal a(n)>0, not appeared previously, such that
 *          p(n)*p(n+1)+a(n) is prime.
 * https://oeis.org/A105692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 56, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the minimal positive integer not yet used such that
   p(n)*p(n+1)+a(n) is prime, where p(k) is the k‑th prime. */

/* The implementation uses a simple sieve for the first 20000 primes,
   a linear list of already used a values, and a cached array of
   computed a(n) terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

/* ------------------------------------------------------------------ */
/* Prime generation -------------------------------------------------- */
static const uint32_t SIEVE_LIMIT = 200000; /* >= 17984th prime */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;
static bool     primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    bool *is_comp = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_comp) return;          /* allocation failure safety */

    prime_list = (uint64_t *)malloc(20000 * sizeof(uint64_t));
    if (!prime_list) { free(is_comp); return; }

    prime_count = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Used a(n) values (linear list) ----------------------------------- */
static seq_t *used_vals = NULL;
static size_t used_cap = 0;
static size_t used_len = 0;

static void used_ensure(void)
{
    if (used_len >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 16;
        seq_t *new_vals = (seq_t *)realloc(used_vals, new_cap * sizeof(seq_t));
        if (!new_vals) return; /* allocation failure safety */
        used_vals = new_vals;
        used_cap = new_cap;
    }
}

static bool used_contains(const seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_vals[i] == v) return true;
    return false;
}

static void used_insert(const seq_t v)
{
    used_ensure();
    used_vals[used_len++] = v;
}

/* ------------------------------------------------------------------ */
/* Sequence cache ---------------------------------------------------- */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0; /* highest index computed (1‑based) */

static seq_t *ensure_seq_slot(size_t n)
{
    if (seq_len < n) {
        size_t needed = n;
        if (!seq_cap || seq_cap < needed) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *new_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
            if (!new_vals) return NULL; /* allocation failure safety */
            seq_vals = new_vals;
            seq_cap = new_cap;
        }
        seq_len = n;
    }
    return &seq_vals[n - 1];
}

/* ------------------------------------------------------------------ */
/* Main function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */
    init_primes();

    /* Extend prime list if required (unlikely for n <= 10000) */
    if ((size_t)n > prime_count) {
        /* Simple extension: generate more primes using a larger sieve */
        uint32_t new_limit = SIEVE_LIMIT * 2;
        bool *is_comp = (bool *)calloc(new_limit + 1, sizeof(bool));
        if (!is_comp) return 0; /* allocation failure */

        size_t extra_capacity = prime_count + 20000;
        uint64_t *new_list = (uint64_t *)realloc(prime_list, extra_capacity * sizeof(uint64_t));
        if (!new_list) { free(is_comp); return 0; }
        prime_list = new_list;

        for (uint32_t i = 2; i <= new_limit; ++i) {
            if (!is_comp[i]) {
                prime_list[prime_count++] = i;
                if ((uint64_t)i * i <= new_limit) {
                    for (uint32_t j = i * i; j <= new_limit; j += i)
                        is_comp[j] = true;
                }
            }
        }
        free(is_comp);
    }

    /* Compute terms up to n if needed */
    for (size_t k = seq_len + 1; k <= (size_t)n; ++k) {
        uint64_t pk  = prime_list[k - 1];   /* p(k)   */
        uint64_t pk1 = prime_list[k];       /* p(k+1) */

        seq_t cand = 1;
        for (;;) {
            seq_t prod = (seq_t)pk * (seq_t)pk1 + cand;
            if (is_prime_seq(prod) && !used_contains(cand)) {
                used_insert(cand);
                *ensure_seq_slot(k) = cand;
                break;
            }
            ++cand;
        }
    }
    return seq_vals[n - 1];
}
