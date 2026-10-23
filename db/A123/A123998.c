/*
 * A123998  Numbers k such that 2k+1 and 4k+1 are primes.
 * https://oeis.org/A123998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123998: numbers k such that 2k+1 and 4k+1 are primes.
   The function keeps a memoised list of found k's and grows it as
   required.  Primality is tested with the supplied is_prime() utility. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* External types defined by the build environment. */
seq_t a(idx_t n)
{
    /* cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* next k to test */
    static idx_t curr_k = 1;

    /* ensure the cache has capacity for at least n terms */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have n of them */
    while (cache_size < n) {
        /* decide whether current k satisfies the condition */
        idx_t two_k_plus1 = curr_k * (idx_t)2 + (idx_t)1;
        idx_t four_k_plus1 = curr_k * (idx_t)4 + (idx_t)1;

        if (is_prime(two_k_plus1) && is_prime(four_k_plus1))
            cache[cache_size++] = (seq_t)curr_k;

        ++curr_k;
    }

    return cache[n - 1];
}
