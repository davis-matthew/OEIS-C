/*
 * A045391  Primes congruent to {1, 2} mod 7.
 * https://oeis.org/A045391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 1 or 2 modulo 7
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1, this case should not happen */

    static seq_t *vals = NULL;   /* array of found primes */
    static size_t cap  = 0;      /* current capacity of vals */
    static size_t sz   = 0;      /* number of primes stored */
    static idx_t next  = 2;      /* next candidate to test (after 2 we use odd numbers) */

    /* Grow the array so that it can hold at least n elements */
    if (sz < n) {
        /* seed the list with the first prime 2 */
        if (sz == 0) {
            if (cap < 1) {
                cap = 8;
                vals = (seq_t *)malloc(cap * sizeof(seq_t));
            }
            vals[0] = (seq_t)2;
            sz = 1;
            next = 3;  /* start checking at the next odd number */
        }

        /* populate more primes until we have n of them */
        while (sz < n) {
            seq_t cand = (seq_t)next;
            if ((next % 7 == 1 || next % 7 == 2) && is_prime_small(cand)) {
                /* ensure space for the new element */
                if (sz >= cap) {
                    size_t ncap = cap ? cap * 2 : 8;
                    while (ncap <= sz) ncap *= 2;
                    seq_t *tmp = (seq_t *)realloc(vals, ncap * sizeof(seq_t));
                    vals = tmp;
                    cap = ncap;
                }
                vals[sz++] = cand;
            }
            next += 2;  /* skip even numbers */
        }
    }
    return vals[n - 1];
}
