/*
 * A172016  Numbers of the form 6*k-+1 such that 6*k-1=nonprime and
 *          6*k+1=prime.
 * https://oeis.org/A172016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A172016: numbers of the form 6*k-1 and 6*k+1 such that
   6*k-1 is composite and 6*k+1 is prime.
   The sequence lists the composite 6*k-1 followed by the prime 6*k+1
   for each qualifying k.  This function computes a(n) for n ≥ 1.
   It maintains a cache of all computed terms so that calls are
   efficient even when n is supplied in arbitrary order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cached terms of the sequence */
    static size_t cache_size  = 0;     /* number of terms stored */
    static size_t cache_cap   = 0;     /* current allocation capacity */
    static idx_t  next_k      = 1;     /* next k to test */

    /* extend the cache until it contains at least n terms */
    while (cache_size < n) {
        /* find the next k such that 6*k-1 is composite and 6*k+1 is prime */
        for (;;) {
            idx_t k      = next_k;
            idx_t low    = 6 * k - 1;
            idx_t high   = 6 * k + 1;
            if (!is_prime(low) && is_prime(high))
                break;          /* found qualifying k */
            ++next_k;
        }

        /* store the pair 6*k-1 and 6*k+1 */
        idx_t k      = next_k;
        seq_t low    = (seq_t)(6 * k - 1);
        seq_t high   = (seq_t)(6 * k + 1);

        /* ensure capacity for two more terms */
        if (cache_size + 2 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < cache_size + 2) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) /* allocation failure – propagate a runtime error */
                abort();
            cache      = tmp;
            cache_cap  = new_cap;
        }

        cache[cache_size++] = low;
        cache[cache_size++] = high;

        ++next_k;
    }

    return cache[n - 1];
}
