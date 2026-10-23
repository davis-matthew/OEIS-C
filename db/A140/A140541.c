/*
 * A140541  Primes of the form 17*k - 1.
 * https://oeis.org/A140541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 397, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime of the form 17*k - 1 by simple search. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit numbers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Undefined for n < 1, but the OIS offset guarantees n >= 1. */
    if (n < 1) return 0;

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static idx_t cached = 0;
    static idx_t capacity = 0;
    static idx_t next_k = 1;  /* k to test next */

    while (cached < n) {
        /* Candidate of the form 17*k - 1. */
        seq_t cand = (seq_t)((uint64_t)17 * (uint64_t)next_k - 1ULL);

        if (is_prime_uint64((uint64_t)cand)) {
            /* Ensure enough space. */
            if (cached == capacity) {
                idx_t new_cap = capacity ? capacity << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);          /* allocation failure */
                cache = tmp;
                capacity = new_cap;
            }
            cache[cached++] = cand;
        }
        ++next_k;
    }
    return cache[n - 1];
}
