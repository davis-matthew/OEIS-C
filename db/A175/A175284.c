/*
 * A175284  Primes p of the form p=n^2+3n+9 such that q=p^2+3p+9 is also
 *          prime.
 * https://oeis.org/A175284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p of the form p = n^2+3n+9 such that q =
 *            p^2+3p+9 is also prime (search for primes, not a closed formula)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"

/* Cache for the sequence terms */
static seq_t *cache = NULL;      /* array of stored primes p */
static size_t cache_size = 0;    /* number of stored terms  */
static size_t cache_cap  = 0;    /* current capacity of the array */
static int64_t next_candidate = -1;  /* next integer n to test for a candidate p */

/* Append a value to the cache, growing the array if necessary. */
static void append_to_cache(seq_t p)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = p;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but return 0 to be safe */

    /* Generate terms until we have enough. */
    while (cache_size < n) {
        /* Candidate index n (can be negative initially). */
        int64_t i = next_candidate++;

        /* Compute p = i^2 + 3*i + 9 using signed 128‑bit arithmetic. */
        __int128 p128 = (__int128)i * i + (__int128)3 * i + 9;
        if (p128 <= 0 || p128 > ( (__int128)UINT64_MAX)) {
            continue; /* out of range or non‑positive */
        }
        uint64_t p = (uint64_t)p128;

        /* Test primality of p. */
        if (!is_prime(p)) continue;

        /* Compute q = p^2 + 3*p + 9. */
        unsigned __int128 q128 = (unsigned __int128)p * p + (unsigned __int128)3 * p + 9;
        if (q128 > (unsigned __int128)UINT64_MAX) {
            continue; /* q too large for is_prime, skip */
        }
        uint64_t q = (uint64_t)q128;

        /* Test primality of q. */
        if (!is_prime(q)) continue;

        /* Store p in the cache. */
        append_to_cache((seq_t)p);
    }

    return cache[n - 1];   /* 1‑based index */
}
