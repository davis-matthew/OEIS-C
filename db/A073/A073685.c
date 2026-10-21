/*
 * A073685  Group the composite numbers so that the sum of each group is a
 *          prime, sequence gives number of terms in each group.
 * https://oeis.org/A073685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the nth block of consecutive composite integers
 *            such that the sum of each block is a prime
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* Cache of previously computed block lengths */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t groups = 0;          /* number of blocks stored */
    static idx_t next   = 3;          /* last integer examined (will be incremented) */
    static idx_t sum    = 0;          /* running sum of the current block */
    static idx_t len    = 0;          /* current block length */

    /* One‑time allocation of the cache */
    if (cap == 0) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely */
    }

    /* Grow the cache when needed */
    if (groups < n && n > (idx_t)cap) {
        size_t new_cap = cap;
        while (n > (idx_t)new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[groups - 1];
        cache = tmp;
        cap = new_cap;
    }

    /* Generate blocks until we have at least n of them */
    while (groups < n) {
        ++next;                     /* move to the next integer */

        /* Skip primes – we only want composites */
        if (isPrime(next))
            continue;

        /* Add this composite to the current block */
        sum += next;
        ++len;

        /* If the running sum is prime, we have completed a block */
        if (isPrime(sum)) {
            cache[groups++] = (seq_t)len;
            sum = 0;
            len = 0;
        }
    }

    return cache[n - 1];
}
