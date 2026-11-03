/*
 * A109921  a(2n) = prime(n). a(2n+1) = sum of composite numbers between
 *          prime(n) and prime(n+1). We define a(1) = 1.
 * https://oeis.org/A109921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 858, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 934 (2026-11-03)
 * Helpers:   is_prime_trial, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 1
 *   a(2n)   = prime(n)
 *   a(2n+1) = sum of composite numbers between prime(n) and prime(n+1)
 *   (i.e. numbers x with prime(n) < x < prime(n+1) that are not prime)
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
    /* handle the special first term */
    if (n == 1)
        return (seq_t)1;

    /* even indices: a(2n) = prime(n) */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;          /* n/2 */
        return get_prime(m);
    }

    /* odd indices: a(2n+1) = sum of composites between prime(n) and prime(n+1) */
    idx_t m = (n - 1) >> 1;        /* (n-1)/2 */
    seq_t p     = get_prime(m);    /* prime(n)   */
    seq_t p_next = get_prime(m + 1);/* prime(n+1) */

    /* Convert to uint64_t for the loop bounds (values are small for the tested range) */
    uint64_t pu     = (uint64_t)p;
    uint64_t pu_next = (uint64_t)p_next;

    seq_t sum = 0;
    for (uint64_t x = pu + 1; x < pu_next; ++x) {
        if (!is_prime_u64(x))
            sum += (seq_t)x;
    }
    return sum;
}
