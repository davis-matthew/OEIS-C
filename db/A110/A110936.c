/*
 * A110936  a(n) = denominator(Bernoulli(prime(n) - 1))/prime(n).
 * https://oeis.org/A110936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = denominator(Bernoulli(prime(n) - 1))/prime(n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 41, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
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
    /* Get the n-th prime (1‑based) as a seq_t and cast to uint64_t.  */
    uint64_t p = (uint64_t)get_prime(n);
    /* m = prime(n) - 1 : small enough to use 64‑bit arithmetic.  */
    uint64_t m = p - 1;
    seq_t denom = 1;

    /* Enumerate divisors of m by trial up to sqrt(m).  */
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* First divisor d.  */
            uint64_t cand = d + 1;          /* prime candidate */
            if (is_prime_u64(cand))
                denom = denom * (seq_t)cand;
            /* Complementary divisor m/d, if distinct.  */
            uint64_t e = m / d;
            if (e != d) {
                cand = e + 1;
                if (is_prime_u64(cand))
                    denom = denom * (seq_t)cand;
            }
        }
    }

    /* Divide by the prime itself – it must divide the denominator.  */
    return denom / (seq_t)p;
}
