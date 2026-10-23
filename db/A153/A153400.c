/*
 * A153400  Numbers k such that 18*k+1 is not prime.
 * https://oeis.org/A153400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k ≥ 0 such that 18*k+1 is not prime. */
/* The sequence is generated incrementally using a simple primality test. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* External linkage required by the build */
seq_t a(idx_t n)
{
    /* Static state to allow random access to the sequence */
    static seq_t *cache = NULL;      /* stored k values (as seq_t)            */
    static size_t cache_cap = 0;     /* capacity of cache                      */
    static size_t cache_size = 0;    /* number of terms currently stored      */
    static idx_t next_k = 0;         /* next k to test (starting from 0)      */

    if (n == 0)                     /* offset is 1; return 0 for safety */
        return (seq_t)0;

    /* Grow cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        /* Compute 18*k + 1 and test primality */
        seq_t m = (seq_t)18 * (seq_t)next_k + (seq_t)1;
        if (!is_prime_big(m)) {
            /* Append next_k into the cache */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
