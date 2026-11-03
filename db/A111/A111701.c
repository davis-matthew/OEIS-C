/*
 * A111701  Least integer obtained when n is divided by prime(1), then by
 *          prime(2), then by prime(3), ..., stopping as soon as one of the
 *          primes does not divide it. In particular, a(2n-1) = 2n-1.
 * https://oeis.org/A111701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n divided successively by 2,3,5,... until a prime does
 *            NOT divide the current value. Equivalent: a(n) = n divided by
 *            the largest primorial that divides n
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9927 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_trial.h"

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
    /* Convert index to the sequence type */
    seq_t curr = (seq_t)n;
    /* get_prime is 1‑based: get_prime(1)=2, get_prime(2)=3, … */
    unsigned long long prime_idx = 1;

    while (1) {
        seq_t p = get_prime(prime_idx);
        ++prime_idx;

        /* If the current value is not divisible by this prime,
           we stop and return the remaining quotient. */
        if (curr % p != 0)
            break;

        /* Divide once by the prime and continue with the next one. */
        curr /= p;

        /* If the result has become 1 we cannot divide further. */
        if (curr == 1)
            break;
    }

    return curr;
}
