/*
 * A111324  Numbers k such that 10 divides prime(1) + ... + prime(k).
 * https://oeis.org/A111324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that sum_{i=1..k}
 *            prime(i) ≡ 0 (mod 10)
 * Range:     u8 n <= 28, u16 n <= 6671, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Array of stored k values (0‑based). */
    static idx_t *k_vals = NULL;   /* positions k where the sum is a multiple of 10 */
    static size_t cap    = 0;      /* allocated capacity of k_vals */
    static size_t size   = 0;      /* number of k values stored */

    /* Incremental prime generation state. */
    static uint64_t cand     = 2;   /* current number to test for primality */
    static uint64_t prime_idx = 0;  /* number of primes seen so far (k index) */
    static uint64_t cum_mod  = 0;   /* cumulative sum modulo 10 */

    /* Generate more terms until we have at least n of them. */
    while (size < (size_t)n) {
        /* Find next prime. */
        while (!is_prime(cand)) {
            ++cand;
        }
        uint64_t p = cand;
        ++cand;                /* advance to next integer (even/odd) */
        ++prime_idx;
        cum_mod += p;
        cum_mod %= 10;
        if (cum_mod == 0) {
            /* Store the current prime index k. */
            if (size == cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                idx_t *tmp = (idx_t*)realloc(k_vals, new_cap * sizeof(idx_t));
                if (!tmp) return (seq_t)0;   /* allocation failure */
                k_vals = tmp;
                cap = new_cap;
            }
            k_vals[size++] = prime_idx;
        }
    }
    return (seq_t)k_vals[n - 1];
}
