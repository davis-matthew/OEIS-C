/*
 * A142950  Primes congruent to 55 mod 64.
 * https://oeis.org/A142950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence: primes congruent to 55 mod 64 (offset 1) */
/* This function returns the n‑th prime p such that p ≡ 55 (mod 64). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of found primes */
    static size_t cached = 0;            /* number of primes stored */
    static size_t capacity = 0;          /* size of cache buffer */
    static idx_t next_k = 0;             /* k for candidate = 64*k + 55 */

    /* Extend the cache until we have at least n primes. */
    while (cached < n) {
        idx_t candidate = 64 * next_k + 55;   /* candidate of the form 64k+55 */

        if (candidate >= 2 && (is_prime)(candidate)) {   /* test for primality */
            /* Grow the cache if necessary. */
            if (cached >= capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                /* cast the result of realloc to seq_t* (valid C/C++). */
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return 0;  /* memory failure – return zero value */
                cache = tmp;
                capacity = newcap;
            }
            cache[cached++] = (seq_t)candidate;   /* store the prime */
        }
        ++next_k;
    }

    return cache[n - 1];   /* zero‑based array index */
}
