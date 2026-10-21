/*
 * A216748  Primes p such that x^34 = -2 has no solution mod p.
 * https://oeis.org/A216748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes an element of the OEIS sequence
   A216748: Primes p such that x^34 = -2 has no solution mod p.
   The sequence consists of those primes p (p > 2) for which
   (-2) is **not** a 34‑th power residue modulo p.
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mod_pow.h"

static seq_t *seq = NULL;        /* array of sequence values          */
static size_t seq_cap = 0;       /* current capacity                  */
static size_t seq_len = 0;       /* number of filled entries          */

static uint64_t *primes = NULL;  /* list of known primes              */
static size_t primes_cap = 0;    /* current capacity                  */
static size_t primes_len = 0;    /* number of filled entries          */

static uint64_t last_checked = 1; /* last integer tested for primality */

/* Allocate or grow a buffer holding seq_t values */
static void ensure_seq_cap(void) {
    if (seq_len + 1 > seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 128 : seq_cap * 2;
        seq_t *new_buf = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq = new_buf;
        seq_cap = new_cap;
    }
}

/* Allocate or grow a buffer holding uint64_t primes */
static void ensure_primes_cap(void) {
    if (primes_len + 1 > primes_cap) {
        size_t new_cap = (primes_cap == 0) ? 128 : primes_cap * 2;
        uint64_t *new_buf = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        primes = new_buf;
        primes_cap = new_cap;
    }
}

/* Trial division using previously found primes.
   Returns true if n is prime. */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    uint64_t limit = (uint64_t) sqrt((double)n) + 1;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > limit) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate the next prime after last_checked.
   Updates last_checked and appends to primes[] */
static uint64_t next_prime(void) {
    while (true) {
        ++last_checked;
        if (last_checked == 2 || (last_checked & 1)) {
            if (is_prime(last_checked)) {
                ensure_primes_cap();
                primes[primes_len++] = last_checked;
                return last_checked;
            }
        }
    }
}

/* Test whether the prime p fails to satisfy the congruence.
   Returns true if p is included in the sequence (i.e. no solution). */
static bool fails_congruence(uint64_t p) {
    uint64_t d;
    if ((p - 1) % 34 == 0) d = 34;
    else if ((p - 1) % 17 == 0) d = 17;
    else d = 2;               /* p-1 always even for odd primes */
    uint64_t exp = (p - 1) / d;
    seq_t base = (seq_t)(p - 2);          /* -2 mod p */
    seq_t val = mod_pow(base, exp, (seq_t)p);
    return val != (seq_t)1;              /* unsolvable => include */
}

seq_t a(idx_t n) {
    /* On first call, initialise primes list to contain 2 */
    if (primes_len == 0) {
        primes_cap = 128;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes_len = 0;
        last_checked = 1;
        /* Ensure 2 is added: */
        primes[primes_len++] = 2;
        last_checked = 2;
    }

    /* Generate primes and test until we have found n sequence values */
    while (seq_len < (size_t)n) {
        uint64_t p = next_prime();
        if (p == 2) continue;          /* skip prime 2 */
        if (fails_congruence(p)) {
            ensure_seq_cap();
            seq[seq_len++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
