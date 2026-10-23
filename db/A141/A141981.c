/*
 * A141981  Primes congruent to 5 mod 29.
 * https://oeis.org/A141981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 180, u32 n <= 191, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 5 (mod 29).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cached primes ≡ 5 (mod 29)  */
static seq_t *targets = NULL;   /* array            */
static size_t targets_len = 0;  /* number of stored primes */
static size_t targets_cap = 0;  /* allocated capacity */

/* Next candidate number of the form 29*k+5 (stored as 64‑bit).  */
static idx_t next_candidate = 5;

/* Ensure the cache contains at least n primes ≡ 5 (mod 29).  */
static void ensure_cache(idx_t n)
{
    if (targets_len >= n) return;

    /* Grow the backing array as needed.  */
    while (targets_len < n) {
        /* Test the current candidate for primality.  */
        seq_t cand = (seq_t)next_candidate;
        if (is_prime_big(cand)) {
            /* Grow if we have reached capacity.  */
            if (targets_len == targets_cap) {
                size_t new_cap = targets_cap ? targets_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(targets, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
                targets = tmp;
                targets_cap = new_cap;
            }
            targets[targets_len++] = cand;
        }
        /* Advance to the next number of the correct residue.  */
        next_candidate += 29;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0; return 0 */
    ensure_cache(n);
    return targets[n - 1];
}
