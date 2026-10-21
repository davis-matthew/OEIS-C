/*
 * A088179  Primes p such that mu(p-1) = 1; that is, p-1 is squarefree and has
 *          an even number of prime factors, where mu is the Moebius function.
 * https://oeis.org/A088179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that μ(p-1) = 1 (i.e. p-1 is
 *            squarefree with an even number of prime factors)
 * Range:     u8 n <= 12, u16 n <= 1233, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mobius.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache of the special primes found so far */
    static seq_t *special = NULL;
    static size_t special_cap = 0;
    static size_t special_count = 0;
    static uint64_t last_prime = 0;   /* last prime checked */

    if (n == 0) { /* offset is 1, n>=1 --> undefined otherwise */
        return 0;
    }

    /* Ensure we have at least n elements in the cache. */
    if (special_count < n) {
        /* initialise cache on first call */
        if (special_count == 0) {
            special_cap = 16;
            special = (seq_t *)malloc(special_cap * sizeof(seq_t));
            if (!special) return 0; /* allocation failure */
            /* handle the first prime 2 */
            special[0] = 2;
            special_count = 1;
            last_prime = 2;
        }

        /* extend cache until we have n elements */
        while (special_count < n) {
            /* get next prime after the last checked */
            last_prime = next_prime(last_prime);
            /* test the Möbius condition */
            if (mobius((idx_t)(last_prime - 1)) == (seq_t)1) {
                /* grow array if necessary */
                if (special_count == special_cap) {
                    special_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(special,
                        special_cap * sizeof(seq_t));
                    if (!tmp) { free(special); return 0; }
                    special = tmp;
                }
                special[special_count++] = (seq_t)last_prime;
            }
        }
    }
    return special[n - 1];
}
