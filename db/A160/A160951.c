/*
 * A160951  Primes p such that 2p + 1155 is prime.
 * https://oeis.org/A160951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that 2p + 1155 is prime, OEIS A160951 */
/* The function searches primes and tests the condition
   2p+1155 for primality. It memoises results in a static array,
   so any ordering of n is supported. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n=0, but not required */

    /* Static cache of found primes */
    static seq_t *list = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t last_checked = 0; /* prime from which to search next */

    /* Lazy initialization of cache */
    if (list == NULL) {
        cap = 16;
        list = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate as many primes as needed */
    while (len < (size_t)n) {
        /* Get next prime candidate */
        last_checked = next_prime(last_checked);

        /* Compute 2p + 1155 and test primality */
        seq_t test = (seq_t)last_checked * (seq_t)2 + (seq_t)1155;
        if (is_prime_big(test)) {
            /* Append to cache, growing array if necessary */
            if (len == cap) {
                cap <<= 1;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
            }
            list[len++] = (seq_t)last_checked;
        }
    }

    return list[n - 1];
}
