/*
 * A045399  Primes congruent to {0, 3, 5, 6} mod 7.
 * https://oeis.org/A045399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_u64_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the n‑th prime p such that p mod 7 ∈ {0,3,5,6}. */
/* The implementation performs an on‑demand sieve of Eratosthenes using
   trial division.  It maintains a cached list of all primes found so far
   and a cached list of the terms of the required subsequence.  Calls to
   a() can be made in any order; the static storage is grown only as
   necessary. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_u64_array.h"

/* Utility: grow an array of seq_t values. */
static void grow_seq_array(seq_t **arr, size_t *cnt, size_t *cap, size_t extra)
{
    if (*cnt + extra > *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap * 2);
        while (new_cap < *cnt + extra) new_cap *= 2;
        seq_t *p = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!p) exit(EXIT_FAILURE);
        *arr = p;
        *cap = new_cap;
    }
}

/* Global state for the on‑demand prime generator. */
static uint64_t *prime_list = NULL;   /* all primes found so far */
static size_t    prime_cnt  = 0;     /* number of primes in prime_list */
static size_t    prime_cap  = 0;

static seq_t    *seq_vals   = NULL;   /* filtered subsequence */
static size_t    seq_cnt    = 0;     /* number of terms in seq_vals */
static size_t    seq_cap    = 0;

static bool      primes_done = false;     /* whether 2 has been processed */
static uint64_t  next_candidate = 3;      /* next odd number to test */

/* Test whether x is prime using the current prime_list. */
static bool is_prime_u64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that at least n terms of the sequence are precomputed. */
static void ensure_terms(idx_t n)
{
    if (seq_cnt >= (size_t)n) return;         /* already sufficient */

    /* Start the prime list with 2 if not already done. */
    if (!primes_done) {
        grow_u64_array(&prime_list, &prime_cnt, &prime_cap, 1);
        prime_list[prime_cnt++] = 2ULL;
        /* 2 is NOT in the sequence because 2 % 7 == 2. */
        primes_done = true;
    }

    /* Generate primes and subsequence terms until we have n terms. */
    while (seq_cnt < (size_t)n) {
        /* Find the next prime. */
        while (!is_prime_u64(next_candidate)) next_candidate += 2;
        /* Add prime to the prime list. */
        grow_u64_array(&prime_list, &prime_cnt, &prime_cap, 1);
        prime_list[prime_cnt++] = next_candidate;

        /* Check the residue condition. */
        uint64_t r = next_candidate % 7ULL;
        if (r == 0ULL || r == 3ULL || r == 5ULL || r == 6ULL) {
            grow_seq_array(&seq_vals, &seq_cnt, &seq_cap, 1);
            seq_vals[seq_cnt++] = (seq_t)next_candidate;
        }

        /* Advance to the next odd candidate. */
        next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; return 0 for safety */

    ensure_terms(n);
    return seq_vals[n - 1];
}
