/*
 * A040040  Average of twin prime pairs (A014574), divided by 2. Equivalently,
 *          2*a(n)-1 and 2*a(n)+1 are primes.
 * https://oeis.org/A040040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1525, u32 n <= 9597, u64 n <= 9597,
 *            u128 n <= 9597, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A040040: the average of twin prime pairs divided by 2.
   That is, if (p, p+2) is the n‑th twin prime pair, a(n) = (p+1)/2. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache to store computed terms so later calls can be handled quickly. */
static seq_t *cache      = NULL;   /* dynamic array of terms */
static size_t cache_sz   = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache */

/* Next odd number to test for twin prime status. */
static uint64_t cur = 3;           /* start with the first odd candidate */

/* Append a new term to the cache, resizing if necessary. */
static void cache_append(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_sz >= cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_sz++] = val;
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return dummy */

    /* Keep generating twin primes until we have at least n terms. */
    while (cache_sz < (size_t)n) {
        /* Test whether cur and cur+2 are primes. */
        if (is_prime(cur) && is_prime(cur + 2)) {
            /* The desired value is (cur+1)/2. */
            seq_t val = ((seq_t)(cur + 1)) / 2;
            cache_append(val);
        }
        cur += 2;   /* move to the next odd candidate */
    }

    return cache[n - 1];
}
