/*
 * A106034  a(n) is the least number such that n*prime(n)+a(n) is a perfect
 *          cube.
 * https://oeis.org/A106034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 0 such that n*prime(n)+k is a perfect cube
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
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
    /* base = n * prime(n) */
    seq_t base = (seq_t)n * get_prime(n);

    /* Binary search for the smallest m with m^3 ≥ base */
    seq_t low = 0;
    seq_t high = base;   /* safe upper bound for the cube root */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        seq_t mid2 = mid * mid;
        seq_t mid3 = mid2 * mid;
        if (mid3 < base) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    /* low is now the cube root floor, and low^3 ≥ base */
    seq_t cube = low * low * low;
    return cube - base;
}
