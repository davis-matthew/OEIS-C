/*
 * A090696  Numbers k such that k^2 - 11 is a prime.
 * https://oeis.org/A090696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 4308, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: a(n) is the n/*th positive integer k such that k²‑11 is prime.
   Cached values are stored on the first call and reused on subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache          = NULL; /* array of already found k values */
static size_t cache_len      = 0;    /* number of terms stored                */
static size_t cache_cap      = 0;    /* current allocation size               */
static idx_t  last_checked   = 3;    /* k that was tested most recently;       */
                                     /* start at 3 so the first candidate is 4 */

/* Return the n-th term (n >= 1) of OEIS A090696. */
seq_t a(idx_t n)
{
    /* Ensure one-time cache allocation. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the sequence until it contains at least n terms. */
    while (cache_len < n) {
        /* Next candidate k. */
        idx_t k = last_checked + 1;

        /* Compute k² – 11 using 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)k * k;
        seq_t val = (seq_t)(prod - 11);

        /* Test primality of k²‑11.  isPrimeGeneric works for all seq_t sizes. */
        if (isPrimeGeneric(val)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)k;   /* store the new term     */
        }

        last_checked = k;  /* remember the most recent k tested */
    }

    return cache[n - 1]; /* terms are 1‑based */
}
