/*
 * A142428  Primes congruent to 17 mod 49.
 * https://oeis.org/A142428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is congruent to 17 modulo 49
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: test numbers of the form 17 + 49·k for primality,
   storing found primes in a dynamically grown static array. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes and state of the search */
    static seq_t *values = NULL;   /* dynamic array of primes */
    static size_t size  = 0;        /* number of primes stored   */
    static size_t cap   = 0;        /* current capacity of array */
    static idx_t  next_k = 0;       /* next k such that candidate = 17 + 49·k */

    if (n == 0)                      /* sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* Initialise the cache lazily. */
    if (values == NULL) {
        cap   = 128;
        values = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!values) exit(1);        /* abort on allocation failure */
    }

    /* Generate primes until we have at least n of them. */
    while (size < n) {
        idx_t candidate_idx = 17 + 49 * next_k;
        ++next_k;

        seq_t candidate = (seq_t)candidate_idx;
        if (is_prime_big(candidate)) {
            /* Ensure enough space in the array. */
            if (size == cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(values, newcap * sizeof(seq_t));
                if (!tmp) exit(1);    /* abort on allocation failure */
                values = tmp;
                cap = newcap;
            }
            values[size++] = candidate;
        }
    }

    return values[n - 1];
}
