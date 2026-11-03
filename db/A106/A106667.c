/*
 * A106667  a(n) = 1 if prime(n) + 2 is a prime, a(n) = -1 if prime(n) + 2 is
 *          a semiprime, otherwise 0.
 * https://oeis.org/A106667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, less
 * Formula:   a(n) = 1 if prime(n)+2 is prime, * -1 if prime(n)+2 is
 *            semiprime, * 0 otherwise * (OEIS A106667)
 * Range:     i8 n <= 30, i16 n <= 3529, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9711 (2026-11-03)
 * Helpers:   is_prime_trial, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_trial.h"
#include "../utils/is_prime_u64.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

seq_t a(idx_t n)
{
    /* get the n‑th prime (indices start at 1) */
    seq_t p = get_prime(n);

    /* candidate to test: prime(n)+2 */
    seq_t cand = p + (seq_t)2;

    /* test for primality */
    if (is_prime_u64((uint64_t)cand))
        return (seq_t)1;

    /* test for semiprimality */
    if (is_semiprime_uint64((uint64_t)cand))
        return (seq_t)(-1);

    /* otherwise 0 */
    return (seq_t)0;
}
