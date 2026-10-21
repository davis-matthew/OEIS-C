/*
 * A052034  Primes such that the sum of the squares of their digits is also a
 *          prime.
 * https://oeis.org/A052034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1265, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_square_sum, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052034: primes whose digits' squares sum to a prime.
   a(n) is the n-th such prime (n≥1). */

/* The sequence is defined by searching for primes p such that
   Σ(digit(p)^2) is also prime.  The algorithm iteratively
   generates successive primes using the provided next_prime
   utility and tests the digit‑square sum with the provided
   is_prime_u64 function.  The results are memoised in a static
   array so that repeated calls are O(1) after the first
   computation. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"
#include "../utils/digit_square_sum.h"

/* Memoised list of found terms. */
static seq_t *terms = NULL;   /* array of seq_t values */
static size_t  used  = 0;     /* number of terms stored */
static size_t  cap   = 0;     /* capacity of allocated array */
static uint64_t last_seen = 1;/* last prime examined */

/* Ensure capacity for at least new_used elements. */
static void ensure_capacity(size_t new_used)
{
    if (new_used <= cap)
        return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < new_used)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    terms = tmp;
    cap = new_cap;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */

    /* If we already have the term, return it. */
    if ((size_t)n <= used)
        return terms[(size_t)(n - 1)];

    /* Generate terms until we reach the desired index. */
    while ((size_t)n > used) {
        /* Find next prime after last_seen. */
        last_seen = next_prime(last_seen);
        if (!last_seen) /* safety guard for overflow */ break;

        /* Compute digit-square sum and test primality. */
        uint64_t sum = digit_square_sum(last_seen);
        if (is_prime_u64(sum)) {
            /* Store this prime in the terms array. */
            ensure_capacity(used + 1);
            terms[used] = (seq_t)last_seen;
            used++;
        }
    }

    /* Return the requested term, if found; otherwise 0. */
    if ((size_t)n <= used)
        return terms[(size_t)(n - 1)];
    return (seq_t)0;
}
