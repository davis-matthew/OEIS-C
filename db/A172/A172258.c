/*
 * A172258  Primes p such that exactly one of the numbers 2p-3 and 2p+3 is
 *          prime.
 * https://oeis.org/A172258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that exactly one of 2p‑3 and 2p+3
 *            is prime
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Append a seq_t value to a dynamic array. */
static void append_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val) {
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Append a uint64_t value to a dynamic array. */
static void append_uint64(uint64_t **arr, size_t *len, size_t *cap, uint64_t val) {
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n) {
    /* Cached data over all calls */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;

    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;

    static uint64_t candidate = 2;          /* next integer to test for primality */

    /* Ensure we have at least n terms computed */
    size_t target = (size_t)n;
    while (terms_cnt < target) {
        /* Find next prime candidate */
        while (!is_prime((idx_t)candidate)) {
            candidate++;
        }

        /* candidate is now a prime */
        uint64_t p = candidate;

        /* Check the condition for this prime */
        seq_t two_p_minus_3 = ((seq_t)2 * (seq_t)p) - (seq_t)3;
        seq_t two_p_plus_3  = ((seq_t)2 * (seq_t)p) + (seq_t)3;
        bool cond = is_prime((idx_t)two_p_minus_3) ^ is_prime((idx_t)two_p_plus_3);

        /* Store prime for possible future use */
        append_uint64(&primes, &primes_cnt, &primes_cap, p);

        if (cond) {
            append_seq(&terms, &terms_cnt, &terms_cap, (seq_t)p);
        }

        /* Move to the next integer for the next search */
        candidate++;
    }

    /* Return the (n-1)-th zero‑indexed element */
    return terms[target - 1];
}
