/*
 * A045424  Primes congruent to {0, 1, 3} mod 7.
 * https://oeis.org/A045424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p such that p == 7 or p%7 == 1 or p%7 ==
 *            3
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to {0,1,3} (mod 7). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Static state for incremental prime generation ---------- */
static uint64_t *prime_list = NULL;   /* all primes found so far (used for trial division) */
static size_t prime_cnt   = 0;        /* number of primes in prime_list               */
static size_t prime_cap   = 0;        /* allocated capacity for prime_list             */

static seq_t *qual_primes = NULL;   /* qualifying primes (0,1,3 mod 7) */
static idx_t  qual_cnt   = 0;        /* number of qualifying primes stored          */
static idx_t  qual_cap   = 0;        /* capacity of qual_primes                       */

static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* Grow prime_list to at least new_cap elements */
static void grow_prime_list(size_t new_cap) {
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) return; /* unlikely, keep previous data intact */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Grow qual_primes to at least new_cap elements */
static void grow_qual_primes(size_t new_cap) {
    seq_t *tmp = (seq_t *)realloc(qual_primes, new_cap * sizeof(seq_t));
    if (!tmp) return;
    qual_primes = tmp;
    qual_cap    = new_cap;
}

/* Test whether x is prime, using trial division by primes already known */
static bool is_prime_uint64(uint64_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > x / p) break;                 /* p*p > x */
        if (x % p == 0) return false;
    }
    return true;
}

/* Append a new prime to prime_list, growing if necessary */
static void push_prime(uint64_t p) {
    if (prime_cnt == prime_cap) {
        size_t new_cap = (prime_cap == 0 ? 16 : prime_cap << 1);
        grow_prime_list(new_cap);
    }
    prime_list[prime_cnt++] = p;
}

/* Append a qualifying prime (as seq_t) to qual_primes, growing if necessary */
static void push_qual_prime(uint64_t p) {
    if (qual_cnt == qual_cap) {
        size_t new_cap = (qual_cap == 0 ? 64 : qual_cap << 1);
        grow_qual_primes(new_cap);
    }
    qual_primes[qual_cnt++] = (seq_t)p;
}

/* Generate primes until we have at least n qualifying primes */
static void ensure_quota(idx_t n) {
    if (qual_cnt >= n) return;

    while (qual_cnt < n) {
        /* Find next prime */
        uint64_t cand = next_candidate;
        while (!is_prime_uint64(cand)) ++cand;
        push_prime(cand);

        /* Check qualification */
        if (cand == 7 || cand % 7 == 1 || cand % 7 == 3)
            push_qual_prime(cand);

        next_candidate = cand + 1;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* safety for out‑of‑range */
    ensure_quota(n);
    return qual_primes[n - 1];
}
