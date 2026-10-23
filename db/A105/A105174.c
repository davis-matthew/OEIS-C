/*
 * A105174  Numbers k such that k*(k+1)/4 - 1 and k*(k+1)/4 + 1 are twin
 *          primes.
 * https://oeis.org/A105174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 579, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105174 : k such that k*(k+1)/4 – 1 and k*(k+1)/4 + 1 are twin primes.
   This implementation performs a simple incremental search, storing found
   terms in a static cache so that any n (in any order) can be answered
   in O(n) time.  Only the provided primality test is used. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *cache   = NULL;   /* array of found k's */
    static idx_t  len     = 0;       /* number of stored terms */
    static idx_t  cap     = 0;       /* capacity of array */
    static idx_t  next_k  = 1;       /* next k to test */

    if (n == 0)                     /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Grow the cache if necessary. */
    if (len < n) {
        /* Ensure we have enough capacity. */
        if (cap < n) {
            idx_t new_cap = (cap == 0) ? 64 : cap * 2;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* Continue searching until we have n terms. */
        while (len < n) {
            /* Only k where k*(k+1)/4 is integer may work. */
            if ((next_k % 4 == 0) || (next_k % 4 == 3)) {
                uint64_t km   = (uint64_t)next_k;
                uint64_t m    = (km * (km + 1)) >> 2;   /* divide by 4 */
                if (m > 1 &&
                    is_prime(m - 1) &&
                    is_prime(m + 1)) {
                    cache[len++] = (seq_t)next_k;
                }
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
