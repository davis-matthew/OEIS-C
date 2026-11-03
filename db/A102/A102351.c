/*
 * A102351  Let p = prime(n); then a(n) = number of residues p mod q which are
 *          prime, as q runs through the primes less than p.
 * https://oeis.org/A102351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q < prime(n) such that prime(n) % q is
 *            prime
 * Range:     u8 n <= 54, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   is_prime_trial, is_prime_u64, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_trial.h"
#include "../utils/is_prime_u64.h"
#include "../utils/simple_sieve.h"

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
    if (n == 0) return 0;  /* offset starts at 1 */

    /* Get the n-th prime */
    uint64_t p_val = (uint64_t)get_prime(n);
    uint32_t p = (uint32_t)p_val;  /* fits in 32 bits for n <= 10000 */

    /* Sieve all primes up to p (including p) */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    simple_sieve(p, &primes, &cnt);

    /* Count prime residues */
    seq_t count = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t q = primes[i];
        if (q >= p) break;
        uint64_t r = p % q;
        if (is_prime_u64(r)) {
            count++;
        }
    }

    free(primes);
    return count;
}
