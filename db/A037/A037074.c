/*
 * A037074  Numbers that are the product of a pair of twin primes.
 * https://oeis.org/A037074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) is the product of the n‑th twin prime pair (p, p+2)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 860, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches the lower members of each twin prime pair found
   so that subsequent calls are O(1).  For very large n a sieve will be
   required, but for n up to about 10 000 the simple linear search
   below is fast enough. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) returns bool */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* undefined for n<1 */

    /* static storage for the lower members of twin prime pairs */
    static uint64_t *lower = NULL;
    static size_t   cap    = 0;
    static size_t   count  = 0;
    static uint64_t cand   = 3;       /* first odd candidate (3 is prime) */

    /* grow the array until we have at least n twin primes */
    if (count < (size_t)n) {
        if (!lower) {
            cap   = 128;
            lower = (uint64_t *)malloc(cap * sizeof(uint64_t));
            if (!lower) return (seq_t)0;   /* allocation failure */
        }

        while (count < (size_t)n) {
            /* find the next prime candidate */
            while (!is_prime(cand)) cand += 2;

            /* if cand+2 is also prime, we have a twin pair */
            if (is_prime(cand + 2)) {
                if (count >= cap) {
                    cap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(lower, cap * sizeof(uint64_t));
                    if (!tmp) return (seq_t)0;   /* allocation failure */
                    lower = tmp;
                }
                lower[count++] = cand;
            }
            cand += 2;   /* advance to the next odd number */
        }
    }

    /* produce p * (p + 2) as a seq_t value */
    seq_t p = (seq_t)lower[n - 1];
    return p * (p + 2);
}
