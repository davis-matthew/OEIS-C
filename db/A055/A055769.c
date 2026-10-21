/*
 * A055769  Largest prime dividing phi of the n-th primorial.
 * https://oeis.org/A055769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{i<=n} [largest_prime_factor(p_i - 1)] where p_i is
 *            the i-th prime. The sequence is computed incrementally: for each
 *            new prime p, factor p-1 to find its largest prime divisor and
 *            update the running maximum. The results are memoised so that
 *            repeated calls for any n are answered in O(1)...
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sqr.h"

/* Local dynamic arrays for the sequence values and the list of primes */
static seq_t *SEQ = NULL;            /* Holds a(1),a(2),... */
static size_t SEQ_LEN = 0, SEQ_CAP = 0;

static uint64_t *PRIMES = NULL;      /* Primes discovered so far */
static size_t PRIMES_LEN = 0, PRIMES_CAP = 0;

/* Grow the sequence array to at least new_cap elements */
static void ensure_seq(size_t new_cap) {
    if (SEQ_CAP < new_cap) {
        size_t cap = SEQ_CAP ? SEQ_CAP * 2 : 8;
        while (cap < new_cap) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(SEQ, cap * sizeof(seq_t));
        if (!tmp) exit(1);
        SEQ = tmp;
        SEQ_CAP = cap;
    }
}

/* Grow the primes array to at least new_cap elements */
static void ensure_primes(size_t new_cap) {
    if (PRIMES_CAP < new_cap) {
        size_t cap = PRIMES_CAP ? PRIMES_CAP * 2 : 8;
        while (cap < new_cap) cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(PRIMES, cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        PRIMES = tmp;
        PRIMES_CAP = cap;
    }
}

/* Return the largest prime factor of m (m>=1).  We use the primes known
   so far, which are all <= m-1.  For m=1 the function returns 1. */
static uint64_t largest_prime_factor(uint64_t m) {
    if (m <= 1) return 1;
    uint64_t largest = 1;
    uint64_t t = m;
    /* Use only primes up to sqrt(t); those are already in PRIMES */
    for (size_t i = 0; i < PRIMES_LEN; ++i) {
        uint64_t p = PRIMES[i];
        if (sqr(p) > t) break;
        if (t % p == 0) {
            largest = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) largest = t;              /* t is now a prime factor > sqrt(m) */
    return largest;
}

/* Generate the next prime after the primes already stored.
   The new prime is appended to PRIMES. */
static void generate_next_prime(void) {
    uint64_t candidate = PRIMES_LEN ? PRIMES[PRIMES_LEN - 1] + 1 : 2;
    for (;;) {
        int is_prime = 1;
        for (size_t i = 0; i < PRIMES_LEN; ++i) {
            uint64_t p = PRIMES[i];
            if (sqr(p) > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) break;
        ++candidate;
    }
    ensure_primes(PRIMES_LEN + 1);
    PRIMES[PRIMES_LEN++] = candidate;
}

/* Append the next a(n) value to SEQ, using the latest prime.
   It assumes that PRIMES[PRIMES_LEN-1] is the new prime. */
static void append_next_term(void) {
    uint64_t prime = PRIMES[PRIMES_LEN - 1];
    uint64_t lpf = largest_prime_factor(prime - 1);
    seq_t prev = SEQ_LEN ? SEQ[SEQ_LEN - 1] : (seq_t)1;
    seq_t next_val = (lpf > (uint64_t)prev) ? (seq_t)lpf : prev;
    ensure_seq(SEQ_LEN + 1);
    SEQ[SEQ_LEN++] = next_val;
}

/* -*- Public function -*- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, 0 is undefined */
    /* Build sequence up to requested index */
    while (SEQ_LEN < n) {
        generate_next_prime();
        append_next_term();
    }
    return SEQ[n - 1];
}
