/*
 * A142612  Primes congruent to 16 mod 55.
 * https://oeis.org/A142612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p with p ≡ 16 (mod 55) */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_small.h"   /* provides bool is_prime_small(uint64_t) */

seq_t a(idx_t n)
{
    if (n == 0) {                /* offset starts at 1 – silently return 0 */
        return (seq_t)0;
    }

    /* Static cache of all primes found so far of this form. */
    static seq_t *primes = NULL;
    static size_t  cap   = 0;
    static size_t  size  = 0;
    static idx_t   next_k = 1;     /* next value of k to produce 55*k+16 */

    /* Allocate or grow the cache as needed. */
    if (size < (size_t)n) {
        size_t needed = n;
        if (cap < needed) {
            size_t new_cap = cap ? cap * 2 : 64;
            while (new_cap < needed) new_cap *= 2;
            seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_arr) abort();          /* unrecoverable – heap exhausted */
            if (primes) {
                memcpy(new_arr, primes, cap * sizeof(seq_t));
                free(primes);
            }
            primes = new_arr;
            cap    = new_cap;
        }

        /* Generate candidates until we have enough primes. */
        while (size < needed) {
            uint64_t cand = 55ULL * next_k + 16ULL;
            if (is_prime_small(cand)) {
                primes[size++] = (seq_t)cand;
            }
            next_k++;
        }
    }

    /* Return the requested element (1‑based indexing). */
    return primes[(size_t)(n - 1)];
}
