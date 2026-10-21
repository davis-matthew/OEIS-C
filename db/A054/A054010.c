/*
 * A054010  Numbers n with property that n is divisible by the number of its
 *          proper divisors.
 * https://oeis.org/A054010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n is divisible by the number of its proper divisors. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Static cache of the sequence values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;
    static idx_t   next   = 2;   /* next integer to test */

    /* Grow the cache until we have at least n terms */
    while (len < n) {
        idx_t m = next;

        /* Count all divisors of m */
        idx_t d = divisor_count_u64((uint64_t)m);
        idx_t proper = d - 1;      /* number of proper divisors */

        /* Check the defining property */
        if (proper > 0 && ((uint64_t)m % (uint64_t)proper) == 0) {
            /* Ensure there is space in the cache */
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = (seq_t)m;        /* store the value */
        }

        ++next;
    }

    return cache[n - 1];
}
