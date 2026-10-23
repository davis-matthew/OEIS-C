/*
 * A142492  Primes congruent to 26 mod 51.
 * https://oeis.org/A142492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 208, u32 n <= 130, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes congruent to 26 mod 51 by checking successive candidates
   51*k + 26 and testing them with is_prime_big.
   The sequence is cached so that calls in any order are answered efficiently. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static void append_prime(seq_t **arr, size_t *cap, size_t *len, seq_t val) {
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static size_t primes_cap = 0, primes_len = 0;
    static idx_t next_k = 0;          /* index of next candidate 51*k+26 */

    while (primes_len < n) {
        /* Build the next candidate.  Use idx_t for calculation to avoid overflow. */
        idx_t candidate_i = (idx_t)51 * next_k + 26;
        seq_t candidate = (seq_t)candidate_i;   /* truncates if seq_t is too small */

        if (is_prime_big(candidate)) {
            append_prime(&primes, &primes_cap, &primes_len, candidate);
        }
        ++next_k;
    }

    return primes[n - 1];
}
