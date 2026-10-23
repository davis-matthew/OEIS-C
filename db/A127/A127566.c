/*
 * A127566  Primes p such that at least one of k-1, k+1 is prime, where k =
 *          absolute value of q^2 - p*r and p, q, r are consecutive primes.
 * https://oeis.org/A127566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 92, u32 n <= 386, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127566
 *   Primes p such that at least one of k − 1 or k + 1 is prime,
 *   where k = |q² − p·r| and p, q, r are consecutive primes.
 *   The function generates primes on demand and caches the found terms.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n) */

/* ---- prime cache ---------------------------------------------------- */
static uint64_t *primes      = NULL;   /* array of primes constructed so far */
static size_t   primes_len   = 0;      /* number of primes in 'primes'      */

/* ---- term cache ----------------------------------------------------- */
static seq_t   *terms         = NULL;  /* array of A127566 terms             */
static size_t   terms_len     = 0;      /* number of terms cached             */
static size_t   terms_cap     = 0;      /* allocated capacity of 'terms'      */

/* index of the first prime of the next consecutive triple */
static idx_t   next_tri_index = 0;

/* --------------------------------------------------------------------- */
static void ensure_terms_cap(void)
{
    if (terms_len < terms_cap) return;
    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(*terms));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------- */
static void ensure_prime_for_index(idx_t idx)
{
    while (primes_len <= (size_t)idx) {
        /* Find the next prime */
        idx_t cand = (primes_len == 0) ? 2 : primes[primes_len - 1] + 1;
        while (!is_prime_big((seq_t)cand)) ++cand;

        /* Store the prime */
        uint64_t *tmp = (uint64_t *)realloc(primes,
                                            (primes_len + 1) * sizeof(*primes));
        if (!tmp) abort();
        primes = tmp;
        primes[primes_len++] = (uint64_t)cand;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* the sequence starts at n = 1 */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Ensure that primes[ next_tri_index + 2 ] exists */
        ensure_prime_for_index(next_tri_index + 2);

        uint64_t p = primes[next_tri_index];
        uint64_t q = primes[next_tri_index + 1];
        uint64_t r = primes[next_tri_index + 2];

        /* k = |q² − p·r|  (k is always even) */
        seq_t qsq  = (seq_t)q * (seq_t)q;
        seq_t prod = (seq_t)p * (seq_t)r;
        seq_t diff = (qsq > prod) ? (qsq - prod) : (prod - qsq);   /* |qsq – prod| */

        /* Check whether k−1 or k+1 is prime */
        bool ok = false;
        if (diff > (seq_t)0) ok = is_prime_big(diff - (seq_t)1);
        if (!ok) ok = is_prime_big(diff + (seq_t)1);

        if (ok) {
            ensure_terms_cap();
            terms[terms_len++] = (seq_t)p;
        }

        ++next_tri_index;   /* move to the next consecutive triple */
    }

    return terms[n - 1];
}
