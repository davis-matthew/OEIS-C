/*
 * A049579  Numbers k such that prime(k)+2 divides (prime(k)-1)!.
 * https://oeis.org/A049579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049579:
 *  Numbers k such that prime(k+1) - prime(k) > 2.
 *  The algorithm generates primes one by one and stores the indices
 *  where the prime gaps exceed 2.  The array is extended only as
 *  needed to answer a(n), so subsequent calls reuse the previously
 *  computed values.  This yields O(n) complexity for the first n
 *  terms and is fast enough for n up to about 10000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static state – persistent between calls */
    static seq_t        *indices = NULL;   /* array of found k's */
    static size_t       cap      = 0;      /* current capacity of the array   */
    static size_t       len      = 0;      /* number of elements currently stored */
    static uint64_t     curr_p   = 2;      /* current prime value              */
    static idx_t        curr_i   = 1;      /* index of current prime           */

    if (n == 0)                /* undefined – return 0 for safety */
        return 0;

    /* index in the array for the requested term (0‑based) */
    idx_t idx = n - 1;

    /* generate terms until we have at least idx+1 entries */
    while (len <= (size_t)idx) {
        /* find the next prime and its index */
        uint64_t next_p   = next_prime(curr_p);
        idx_t     next_i  = curr_i + 1;

        /* if the gap between the two consecutive primes is >2,
         * record the index of the lower prime (curr_i) */
        if ((seq_t)(next_p - curr_p) > (seq_t)2) {
            /* ensure there is space for another element */
            if (len == cap) {
                size_t new_cap = cap ? (cap << 1) : 1;
                seq_t *tmp = (seq_t *)realloc(indices, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failure – abort gracefully */
                    return 0;
                indices = tmp;
                cap = new_cap;
            }
            indices[len++] = (seq_t)curr_i;
        }

        /* advance to the next prime for the next iteration */
        curr_p = next_p;
        curr_i = next_i;
    }

    return indices[(size_t)idx];
}
