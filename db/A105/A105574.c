/*
 * A105574  a(1) = 2; for n > 1, a(n) is the prime whose index is the least
 *          prime factor of n.
 * https://oeis.org/A105574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = the n-th prime, where n is the smallest prime factor of
 *            the argument (with a(1) = 2)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 86 terms through n = 9838 (2026-11-01)
 * Helpers:   is_prime_trial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_trial.h"
#include "../utils/isqrt.h"

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

/* Compute the smallest prime factor of n (n >= 2). */
static inline idx_t smallest_prime_factor(idx_t n)
{
    if ((n & 1U) == 0) return 2;               /* even number */
    idx_t limit = isqrt(n);
    for (idx_t i = 3; i <= limit; i += 2) {
        if (n % i == 0) return i;
    }
    return n;        /* n is prime */
}

/* OEIS sequence A105574 */
seq_t a(idx_t n)
{
    if (n == 1) return 2;                     /* by definition */
    idx_t spf = smallest_prime_factor(n);     /* smallest prime factor of n */
    return get_prime(spf);                    /* prime with index = spf */
}
