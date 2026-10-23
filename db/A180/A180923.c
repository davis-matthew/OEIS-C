/*
 * A180923  Numbers n such that 111*n + 1 is prime.
 * https://oeis.org/A180923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180923: Numbers n such that 111*n + 1 is prime. */
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

/* Indexed from 1; returns the n-th positive integer m such that 111*m+1 is prime. */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of cached terms */
    static size_t capacity   = 0;      /* current allocation size of cache */
    static idx_t last_checked = 1;     /* next integer to test for primality */

    /* Extend cache until we have at least n terms */
    while (cache_size < n)
    {
        /* test successive integers until a new term is found */
        for (idx_t m = last_checked; ; ++m)
        {
            seq_t value = ((seq_t)111) * (seq_t)m + 1;
            if (is_prime_small(value))
            {
                /* grow cache if necessary */
                if (cache_size + 1 > capacity)
                    ensure_capacity(cache_size + 1, &capacity, (void **)&cache, sizeof(seq_t));

                cache[cache_size++] = m;  /* store the found term */
                last_checked = m + 1;     /* next search starts after this m */

                /* if we reached required term, stop */
                if (cache_size == n)
                    return m;
                break; /* break for-loop to continue next extension if needed */
            }
        }
    }

    /* cache_size >= n, return stored value */
    return cache[n - 1];
}
