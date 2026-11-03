/*
 * A101206  Third column of triangle in A101204.
 * https://oeis.org/A101206
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! - 2^(prime(n-3)-2) for n ≥ 4, a(2) = 1, a(3) = 2
 *            (the first terms)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 6 (2026-11-01)
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

/* Computes a(n) for the OEIS sequence A101206. */
seq_t a(idx_t n)
{
    /* Handle the small offsets explicitly. */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Compute (n-1)! in seq_t. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* Compute exponent: prime(n-3) - 2.  get_prime(idx) returns seq_t. */
    seq_t prime_val = get_prime(n - 3);
    /* Convert to idx_t for seq_pow; the prime fits in 64‑bit. */
    idx_t exp = (idx_t)(prime_val) - 2;

    /* Compute 2^exp. */
    seq_t pow2 = ipow_idx((seq_t)2, exp);

    /* a(n) = fact - pow2. */
    return fact - pow2;
}
