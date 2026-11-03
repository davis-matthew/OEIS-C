/*
 * A107060  a(n) = smallest number m >=0 such that n*prime(n)-m is a prime.
 * https://oeis.org/A107060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ≥ 0 such that n*prime(n) - m is prime
 * Range:     u8 n <= 9, u16 n <= 795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9444 (2026-11-03)
 * Helpers:   is_prime_trial, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* Compute n * prime(n).  The result fits in seq_t for all n up to the limits
       considered (n ≤ 10 000 ⇒ n*prime(n) < 1.1·10⁹).                     */
    seq_t p = get_prime(n);
    seq_t prod = (seq_t)n * p;

    /* Search downward for the largest prime ≤ prod.  The counter is kept as
       uint64_t because prod is small enough.                                */
    uint64_t prod64 = (uint64_t)prod;
    uint64_t cand = prod64;
    while (cand >= 2 && !is_prime_u64(cand))
        --cand;
    if (cand < 2) cand = 2;            /* safety: 2 is the smallest prime */

    /* The desired m is the difference between prod and the found prime.   */
    seq_t m = prod - (seq_t)cand;
    return m;
}
