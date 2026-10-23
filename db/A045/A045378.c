/*
 * A045378  Primes congruent to {2, 4} mod 5.
 * https://oeis.org/A045378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Incrementally search integer by integer, testing each
   candidate with is_prime_big.  Collect those primes that are
   congruent to 2 or 4 modulo 5.  Uses static arrays to memoise
   already found terms so that calls may appear in any order. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cached filtered primes (≡ 2 or 4 mod 5). */
static seq_t *filtered = NULL;
static size_t filt_count = 0, filt_cap = 0;

/* Next integer to be tested for primality. */
static seq_t next_candidate = 2;

/* Ensure capacity for the next element of filtered. */
static void ensure_filter_capacity(void)
{
    if (filt_cap <= filt_count) {
        size_t new_cap = filt_cap ? filt_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(filtered, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        filtered = tmp;
        filt_cap = new_cap;
    }
}

/* Return the n‑th prime congruent to 2 or 4 (mod 5). */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term. */
    while (filt_count < n) {
        if (is_prime_big(next_candidate)) {
            if ((next_candidate % 5 == 2) || (next_candidate % 5 == 4)) {
                ensure_filter_capacity();
                filtered[filt_count++] = next_candidate;
            }
        }
        ++next_candidate;
    }
    return filtered[n - 1];
}
