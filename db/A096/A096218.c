/*
 * A096218  Primes of the form 3*prime(n) + 2*prime(n+1).
 * https://oeis.org/A096218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*p(k) + 2*p(k+1), where p(k) is the k-th prime. The
 *            function returns the n-th term (n ≥ 1)
 * Range:     u8 n <= 12, u16 n <= 347, u32 n <= 416, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"    /* uint64_t next_prime(uint64_t start); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

/* Static state shared between calls */
static seq_t *terms = NULL;            /* stored sequence values */
static size_t term_count = 0;          /* number of terms computed so far */
static size_t terms_cap = 0;           /* allocated capacity for terms */
static uint64_t p1 = 0;                /* current prime p(k)   */
static uint64_t p2 = 0;                /* next prime p(k+1)   */
static bool initialized = false;      /* whether primes are seeded */

/* Ensure enough capacity for the terms array */
static void grow_terms(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE); /* out of memory */
    terms = tmp;
    terms_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for 0 */

    /* Seed the first two primes on first call */
    if (!initialized) {
        p1 = 2;
        p2 = next_prime(p1);   /* next prime > 2  */
        initialized = true;
    }

    /* Generate terms until we have n of them */
    while (term_count < (size_t)n) {
        /* Compute candidate value 3*p1 + 2*p2 as seq_t */
        seq_t val = (seq_t)3 * p1 + (seq_t)2 * p2;

        /* If the candidate itself is prime, store it */
        if (is_prime_big(val)) {
            grow_terms(term_count + 1);
            terms[term_count++] = val;
        }

        /* Advance to the next consecutive prime pair */
        p1 = p2;
        p2 = next_prime(p1);
    }

    return terms[(size_t)n - 1];
}
