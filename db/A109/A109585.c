/*
 * A109585  a(n) = ( prime(n+1) - prime(n) )^(n+1).
 * https://oeis.org/A109585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1)-prime(n))^(n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 470 (2026-11-01)
 * Helpers:   ipow_idx, is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
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
    /* Get consecutive primes p_n and p_{n+1} */
    seq_t p1 = get_prime(n);
    seq_t p2 = get_prime(n + 1);

    /* Difference of the primes */
    seq_t diff = p2 - p1;

    /* Raise the difference to the (n+1)-th power */
    return ipow_idx(diff, n + 1);
}
