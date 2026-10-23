/*
 * A102544  Numbers k such that k777777 is prime.
 * https://oeis.org/A102544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 6835, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102544: numbers k such that k777777 is prime.
   The function generates terms on demand by testing successive k
   for primality of k*10^6 + 777777. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset, return 0 */

    /* static state to allow calls in any order */
    static idx_t found = 0;          /* how many terms already found */
    static seq_t *cache = NULL;     /* array of found terms */
    static idx_t capacity = 0;      /* current allocated capacity */
    static uint64_t last_k = 0;     /* last tested k */

    /* grow the cache until we have at least n elements */
    while (found < n) {
        /* allocate or grow the cache */
        if (!cache) {
            capacity = 64;
            cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        } else if (found >= capacity) {
            capacity <<= 1;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        }

        /* test successive k values */
        ++last_k; /* next candidate k */
        uint64_t cand = last_k * 1000000ULL + 777777ULL;
        if (is_prime_uint64(cand)) {
            cache[found] = (seq_t)last_k;
            ++found;
        }
    }

    return cache[n - 1];
}
