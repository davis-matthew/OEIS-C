/*
 * A109805  a(n) = prime(n+2)*prime(n+1) - prime(n)*prime(n+1).
 * https://oeis.org/A109805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n+2)*prime(n+1) - prime(n)*prime(n+1) (offset 1)
 * Range:     u8 n <= 10, u16 n <= 325, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 996 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
    /* For n < 1 the sequence is undefined; return 0 to avoid invalid indices. */
    if (n < 1)
        return 0;

    /* Retrieve the needed primes. */
    seq_t p_n   = get_prime((uint64_t)n);       /* prime(n)  */
    seq_t p_n1  = get_prime((uint64_t)(n + 1)); /* prime(n+1) */
    seq_t p_n2  = get_prime((uint64_t)(n + 2)); /* prime(n+2) */

    /* a(n) = prime(n+2)*prime(n+1) - prime(n)*prime(n+1) */
    return p_n2 * p_n1 - p_n * p_n1;
}
