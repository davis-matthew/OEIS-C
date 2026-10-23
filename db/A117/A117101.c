/*
 * A117101  Numbers k such that nextprime(5*k) > 5*nextprime(k) and k is
 *          composite.
 * https://oeis.org/A117101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 8896, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A117101: numbers k such that k is composite
   and nextprime(5*k) > 5*nextprime(k).  a(1) is the first listed term. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

static seq_t *cache = NULL;      /* cached terms of the sequence */
static size_t cached = 0;        /* number of terms stored */
static size_t capacity = 0;      /* allocated capacity for 'cache' */
static uint64_t last_checked = 0;/* last integer k examined, 0-based */

/* Extend the cache until it contains at least n terms. */
static void grow_cache_to_n(idx_t n)
{
    while (cached < (size_t)n) {
        last_checked += 1;                     /* candidate integer k */

        /* k must be composite */
        if (is_prime(last_checked))
            continue;

        /* Compute next primes after k and after 5*k */
        uint64_t pk  = next_prime(last_checked);
        uint64_t pk5 = next_prime(5ULL * last_checked);

        /* Test the inequality nextprime(5*k) > 5*nextprime(k) */
        unsigned __int128 rhs = (unsigned __int128)5 * pk;
        if ((unsigned __int128)pk5 <= rhs)
            continue;                           /* condition not met */

        /* Ensure enough space for the new term */
        if (cached == capacity) {
            size_t new_cap = (capacity == 0) ? 4 : capacity * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)                            /* allocation failure */
                return;
            cache = tmp;
            capacity = new_cap;
        }
        cache[cached++] = (seq_t)last_checked;    /* store the term */
    }
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset < 1, safe default */

    grow_cache_to_n(n);           /* ensure cache has at least n terms */
    return cache[(size_t)n - 1];  /* array index is 0‑based */
}
