/*
 * A154384  Odd nonprimes with odd sum of digits.
 * https://oeis.org/A154384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A154384: 
   odd nonprimes whose decimal digit sum is odd.
   The function enumerates odd numbers, tests for compositeness
   (or the special case 1), checks the parity of the digit sum,
   and caches the results so that subsequent calls are fast. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;   /* number of terms stored */
    static size_t cache_cap = 0; /* capacity of cache array */
    /* next odd candidate to test (starts at 1) */
    static uint64_t next_candidate = 1;

    /* Return cached value if already computed */
    if (n <= cache_sz)
        return cache[n-1];

    /* Ensure cache has enough space */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */
    }

    /* Generate terms until we reach index n */
    uint64_t candidate = next_candidate;
    while (cache_sz < n) {
        /* candidate is odd by construction */
        bool composite;
        if (candidate == 1)
            composite = true;  /* 1 is not prime */
        else
            composite = !is_prime_small((uint64_t)candidate);

        if (composite) {
            /* compute digit sum */
            uint64_t t = candidate;
            uint64_t sum = 0;
            while (t) {
                sum += t % 10;
                t /= 10;
            }
            if ((sum & 1ULL) != 0) {  /* odd digit sum */
                if (cache_sz == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) abort();
                }
                cache[cache_sz++] = (seq_t)candidate;
            }
        }

        candidate += 2; /* next odd number */
    }

    /* Update the next candidate for future calls */
    next_candidate = candidate;

    return cache[n-1];
}
