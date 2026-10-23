/*
 * A104776  Numbers n such that n+2*prime(n) is a perfect square.
 * https://oeis.org/A104776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   uint64_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104776: a(n) = the n‑th integer k such that k + 2*prime(k) is a perfect square */
/* The function generates terms on demand using an incremental prime generator and a square test. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/uint64_isqrt.h"

/* Global state for primes and sequence terms */
static struct {
    idx_t prime_cap;
    idx_t prime_count;
    uint64_t *primes;

    idx_t terms_cap;
    idx_t terms_count;
    seq_t *terms;

    idx_t next_candidate;   /* next n to test */
    idx_t last_checked;     /* last integer checked for primality during prime list expansion */
} g = {0};

static void init_state(void) {
    static bool init = false;
    if (init) return;
    init = true;

    g.prime_cap = 128;
    g.primes = (uint64_t*)malloc(g.prime_cap * sizeof(uint64_t));
    g.prime_count = 0;
    g.last_checked = 1;

    g.terms_cap = 128;
    g.terms = (seq_t*)malloc(g.terms_cap * sizeof(seq_t));
    g.terms_count = 0;

    g.next_candidate = 1;
}

/* Test if n is prime using the primes already generated */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    for (idx_t i = 0; i < g.prime_count; ++i) {
        uint64_t p = g.primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate sequence terms up to at least target_index */
static void generate_until(idx_t target_index) {
    init_state();

    while (g.terms_count < target_index) {
        /* Ensure we have the prime of index g.next_candidate */
        while (g.prime_count < g.next_candidate) {
            /* Find next prime after g.last_checked */
            uint64_t cand;
            do {
                g.last_checked++;
                cand = g.last_checked;
            } while (!is_prime_uint64(cand));

            /* Store new prime */
            if (g.prime_count == g.prime_cap) {
                g.prime_cap *= 2;
                g.primes = (uint64_t*)realloc(g.primes,
                                             g.prime_cap * sizeof(uint64_t));
            }
            g.primes[g.prime_count++] = cand;
        }

        /* Compute value for current candidate */
        uint64_t cand = g.next_candidate;
        uint64_t prime_val = g.primes[g.next_candidate - 1];
        uint64_t s = cand + 2 * prime_val;
        idx_t r = uint64_isqrt(s);
        if (r * r == s) {
            /* Store term */
            if (g.terms_count == g.terms_cap) {
                g.terms_cap *= 2;
                g.terms = (seq_t*)realloc(g.terms,
                                          g.terms_cap * sizeof(seq_t));
            }
            g.terms[g.terms_count++] = (seq_t)cand;
        }
        g.next_candidate++;
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    generate_until(n);
    return g.terms[n - 1];
}
