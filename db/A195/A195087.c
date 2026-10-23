/*
 * A195087  Numbers k such that (number of prime factors of k counted with
 *          multiplicity) less (number of distinct prime factors of k) = 3.
 * https://oeis.org/A195087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that (total prime factors counted with multiplicity)
   minus (number of distinct prime factors) equals 3
   (i.e. the sum of (exponent‑1) over the prime factorisation of k is 3).
   This routine searches integers in increasing order, keeps a static
   cache of the discovered terms and returns the n‑th such integer.
   The cache allows repeated calls for arbitrary n without restarting
   the search.  The implementation uses only standard C facilities
   (no external utility functions). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static dispatcher state: cache of found terms and next candidate. */
    static seq_t *cache = NULL;       /* array of found terms */
    static idx_t  cache_size = 0;     /* number of terms currently cached */
    static size_t cache_cap  = 0;     /* allocated capacity of the array */
    static seq_t  next_candidate = 2; /* next integer to examine */
    static bool   initialized = false;

    /* One‑time initialization of the cache buffer. */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);          /* abort on allocation failure */
        cache_size = 0;
        initialized = true;
    }

    /* Grow cache array if a new term will be added. */
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);            /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Main search loop: continue until we have found at least n terms. */
    while (cache_size < n) {
        uint64_t tmp = (uint64_t)next_candidate;  /* 64‑bit value for factorisation */
        int    extra = 0;                         /* sum of (exponent-1) */

        /* Factorisation by trial division.  Early exit if extra > 3. */
        if (tmp % 2 == 0) {
            int e = 0;
            while (tmp % 2 == 0) { tmp /= 2; ++e; }
            if (e > 1) { extra += e - 1; }
        }

        for (uint64_t p = 3; p * p <= tmp; p += 2) {
            if (tmp % p == 0) {
                int e = 0;
                while (tmp % p == 0) { tmp /= p; ++e; }
                if (e > 1) { extra += e - 1; }
            }
            if (extra > 3) break;   /* no need to keep factoring */
        }

        /* Remaining tmp > 1 is a prime factor with exponent 1, contributes 0. */

        if (extra == 3) {
            cache[cache_size++] = next_candidate;
            /* Ensure the cache buffer can still grow for future terms. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];   /* 1‑based indexing as required by OEIS */
}
