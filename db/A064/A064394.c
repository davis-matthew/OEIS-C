/*
 * A064394  Numbers k such that the exponent of highest power of 2 dividing k!
 *          equals the largest prime < k.
 * https://oeis.org/A064394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 3560, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   v2_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064394 – k such that v2(k!) = largest prime < k.
   v2(k!) is computed as the sum of floor(k/2^i), which equals k - popcount(k)
   for 64‑bit values.  The sequence is generated lazily; each call to a(n)
   causes a scan of further integers until the nth term is found.
   The implementation uses a dynamic sieve for primality testing and
   stores only the terms that have been requested so far. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/v2_factorial.h"

/* Global storage for the computed terms. */
static seq_t *terms = NULL;
static size_t term_len = 0;
static size_t term_cap = 0;

/* Sieve of Eratosthenes for primality testing. */
static bool *is_prime = NULL;
static idx_t prime_limit = 0;

/* Next integer to be tested for membership in the sequence. */
static idx_t next_k = 4;

/* Ensure the terms array has at least 'needed' elements of capacity. */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(EXIT_FAILURE);
    terms = new_ptr;
    term_cap = new_cap;
}

/* Rebuild the sieve up to at least 'new_limit'.  The sieve array is sized
   as new_limit+1 with indices 0..new_limit. */
static void rebuild_sieve(idx_t new_limit)
{
    if (new_limit <= prime_limit)
        return;
    bool *new_is_prime = (bool *)calloc((size_t)new_limit + 1, 1);
    if (!new_is_prime)
        exit(EXIT_FAILURE);
    for (idx_t i = 2; i <= new_limit; ++i)
        new_is_prime[i] = true;
    for (idx_t i = 2; (idx_t)((unsigned long long)i * i) <= new_limit; ++i) {
        if (new_is_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_is_prime[j] = false;
        }
    }
    free(is_prime);
    is_prime = new_is_prime;
    prime_limit = new_limit;
}

/* Return the largest prime < k; returns 0 if none (k <= 2). */
static idx_t prev_prime(idx_t k)
{
    if (k <= 2)
        return 0;
    for (idx_t x = k - 1; x >= 2; --x) {
        if (is_prime[x])
            return x;
    }
    return 0;
}

/* Advance the global search until 'target_n' terms have been found. */
static void compute_terms_upto(idx_t target_n)
{
    while (term_len < (size_t)target_n) {
        /* Ensure the sieve covers the current candidate. */
        if (next_k > prime_limit)
            rebuild_sieve(next_k * 2);

        idx_t v2 = v2_factorial(next_k);
        idx_t pp = prev_prime(next_k);

        if (v2 == pp) {
            ensure_terms_cap(term_len + 1);
            terms[term_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* The required public function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1; undefined for n = 0 */
        return (seq_t)0;
    compute_terms_upto(n);
    return terms[n - 1];
}
