/*
 * A153129  Numbers n such that 8*n + 5 is not prime.
 * https://oeis.org/A153129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153129: Numbers n such that 8*n + 5 is not prime.
   The sequence is obtained by iterating over positive integers,
   skipping those for which 8*n+5 is prime.  The returned value a(n)
   is the n-th such integer, with a(1)=2. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Guard against n==0; behaviour below the offset is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* Next candidate integer to test. */
    static idx_t candidate = 1;

    /* Generate terms until cache_len reaches n. */
    while (cache_len < (size_t)n) {
        /* Grow cache if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Out of memory: abort gracefully. */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Check whether 8*candidate + 5 is composite. */
        idx_t val = (idx_t)8 * candidate + (idx_t)5;
        if (!is_prime(val)) {
            cache[cache_len++] = (seq_t)candidate;
        }
        candidate++;
    }

    return cache[n - 1];
}
