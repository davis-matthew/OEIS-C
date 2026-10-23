/*
 * A163504  a(n) = abs(n-th prime minus n-th odd nonprime).
 * https://oeis.org/A163504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(A000040(n)-A014076(n))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A(n) is the absolute difference between the n‑th prime        */
/* and the n‑th odd non‑prime (including 1).                     */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static size_t prime_cap = 0;           /* current capacity of prime_list */
static seq_t *prime_list = NULL;       /* array of primes, type seq_t */
static size_t prime_count = 0;         /* number of primes stored */
static uint64_t next_prime_candidate = 3;   /* next odd to test for prime */
static bool primes_initialized = false;     /* flag to initialize prime 2 */

static size_t odd_np_cap = 0;          /* capacity of odd_np_list */
static seq_t *odd_np_list = NULL;      /* array of odd non‑primes */
static size_t odd_np_count = 0;        /* number of entries stored */
static uint64_t odd_np_candidate = 3;  /* next odd to test for non‑prime */
static bool odd_np_initialized = false;    /* flag to init 1 */

/* ------------------------------------------------------------------ */
/* Ensure that prime_list holds at least n primes (1‑based indexing).  */
/* ------------------------------------------------------------------ */
static void
ensure_primes(idx_t n)
{
    if (!primes_initialized) {
        /* include the first prime, 2 */
        prime_cap = 16;
        prime_list = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        prime_list[0] = (seq_t)2;
        prime_count = 1;
        next_prime_candidate = 3;
        primes_initialized = true;
    }

    while (prime_count < n) {
        if (is_prime(next_prime_candidate)) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap * 2;
                prime_list = (seq_t *)realloc(prime_list,
                                              prime_cap * sizeof(seq_t));
            }
            prime_list[prime_count++] = (seq_t)next_prime_candidate;
        }
        next_prime_candidate += 2;   /* only odd numbers */
    }
}

/* ------------------------------------------------------------------ */
/* Ensure that odd_np_list holds at least n odd non‑prime numbers      */
/* (odd numbers > 0 that are not prime, 1 is considered the first).  */
/* ------------------------------------------------------------------ */
static void
ensure_odd_nonprimes(idx_t n)
{
    if (!odd_np_initialized) {
        odd_np_cap = 16;
        odd_np_list = (seq_t *)malloc(odd_np_cap * sizeof(seq_t));
        odd_np_list[0] = (seq_t)1;      /* 1 is the first odd non‑prime */
        odd_np_count = 1;
        odd_np_candidate = 3;
        odd_np_initialized = true;
    }

    while (odd_np_count < n) {
        if (!is_prime(odd_np_candidate)) {
            if (odd_np_count == odd_np_cap) {
                odd_np_cap = odd_np_cap * 2;
                odd_np_list = (seq_t *)realloc(odd_np_list,
                                               odd_np_cap * sizeof(seq_t));
            }
            odd_np_list[odd_np_count++] = (seq_t)odd_np_candidate;
        }
        odd_np_candidate += 2;   /* skip even numbers */
    }
}

/* ------------------------------------------------------------------ */
/* Public function implementing the sequence.                         */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for n < 1, safe fallback */
    }

    ensure_primes(n);
    ensure_odd_nonprimes(n);

    seq_t prime_val  = prime_list[n - 1];
    seq_t odd_np_val = odd_np_list[n - 1];

    return (prime_val > odd_np_val) ? (prime_val - odd_np_val)
                                   : (odd_np_val - prime_val);
}
