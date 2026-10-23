/*
 * A045464  Primes congruent to {0, 6} mod 7.
 * https://oeis.org/A045464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A045464: primes congruent to 0 or 6 mod 7.
    a(1) = 7, a(2) = 13, a(3) = 41, …  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return the n-th prime p such that p mod 7 = 0 or 6. */
seq_t a(idx_t n)
{
    /* Static cached data across calls. */
    static idx_t *cached = NULL;     /* array of found primes */
    static size_t cap = 0;           /* capacity of array */
    static size_t len = 0;           /* number of primes stored */
    static idx_t last_checked = 1;   /* last integer examined */

    /* Expand the cache until we have at least n elements. */
    while (len < n) {
        last_checked++;  /* increment candidate */

        /* Only consider numbers that are 0 or 6 modulo 7. */
        if (last_checked == 7 || last_checked % 7 == 6) {
            if (!is_prime(last_checked))
                continue;   /* skip composites */

            /* Grow the cache if necessary. */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                idx_t *p = (idx_t *)realloc(cached, new_cap * sizeof(idx_t));
                if (!p) exit(1);   /* allocation failure */
                cached = p;
                cap = new_cap;
            }

            /* Store the found prime. */
            cached[len++] = last_checked;
        }
    }

    /* Return the (n-1)-th element cast to seq_t. */
    return (seq_t)cached[n - 1];
}
