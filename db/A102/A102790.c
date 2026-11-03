/*
 * A102790  Integer part of n#/(p-3)#, where p=preceding prime to n.
 * https://oeis.org/A102790
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * (prime(n)-2 if prime(n)-2 is prime else 1)
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_trial, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
    // undefined for n < 2, return 0 for safety
    if (n < 2) return (seq_t)0;

    // Obtain nth prime
    seq_t p = get_prime((uint64_t)n);

    // Convert to uint64_t to test for primality of p-2
    uint64_t p_val   = (uint64_t)p;
    uint64_t pm2_val = (p_val > 2) ? p_val - 2 : 0;

    // If p-2 is prime, multiply by it; otherwise return p
    if (pm2_val > 2 && is_prime_u64(pm2_val))
        return p * (seq_t)pm2_val;
    else
        return p;
}
