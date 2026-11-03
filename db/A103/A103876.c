/*
 * A103876  a(n) = -1/10 (mod prime(n)): A test for divisibility by the n-th
 *          prime.
 * https://oeis.org/A103876
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p - (10 mod p)^(-1) where p = prime(n)
 * Range:     u8 n <= 54, u16 n <= 6756, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9614 (2026-11-01)
 * Helpers:   is_prime_trial, modinv_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_trial.h"
#include "../utils/modinv_idx_to_seq.h"

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
    /* 1. Obtain the n‑th prime (OEIS indexing starts at 1). */
    seq_t p = get_prime(n);

    /* 2. Compute the modular inverse of 10 modulo p. */
    /*    The helper modinv takes idx_t arguments and returns seq_t. */
    seq_t inv = modinv_idx_to_seq((idx_t)10, (idx_t)p);

    /* 3. The desired term is p - inv (always non‑negative). */
    return p - inv;
}
