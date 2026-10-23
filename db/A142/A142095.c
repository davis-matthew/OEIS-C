/*
 * A142095  Primes congruent to 29 mod 35.
 * https://oeis.org/A142095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 29 (mod 35).  The function returns the nth term
   of OEIS A142095, with offset 1.  The implementation keeps a cache
   of already-queried primes and extends it as needed.  Candidates are
   generated as 29, 64, 99, ... (i.e. 29 + 35*k) and accepted if
   they are prime using isPrimeGeneric. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n) */

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of values stored */
    static idx_t cache_capacity = 0; /* current allocation size */
    static seq_t next_candidate;     /* next number to test */
    static bool initialized = false; /* ensure one-time init */

    if (n == 0) return 0; /* offset is 1, but caller should not request n=0 */

    /* Return cached value if already computed */
    if (cache && n <= cache_size)
        return cache[n - 1];

    /* Initialisation */
    if (!initialized) {
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */
        cache_size = 0;
        next_candidate = 29;
        initialized = true;
    }

    /* Grow cache until we have at least n elements */
    while (cache_size < n) {
        /* Test current candidate */
        if (isPrimeGeneric(next_candidate)) {
            /* Ensure enough space */
            if (cache_size >= cache_capacity) {
                idx_t new_cap = cache_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        next_candidate += 35; /* next number in the 29 mod 35 progression */
    }

    return cache[n - 1];
}
