/*
 * A142327  Primes congruent to 29 mod 45.
 * https://oeis.org/A142327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 269, u32 n <= 169, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth prime congruent to 29 (mod 45). */
/* The function searches candidates of the form 45*k + 29 and tests for primality. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The offset is 1, so a(1) is the first prime of the form 45k+29. */
    if (n == 0)
        return (seq_t)0;   /* undefined behaviour for n < 1, return 0 */

    /* Static cache of found primes. */
    static seq_t *primes = NULL;
    static size_t capacity = 0;
    static size_t size = 0;          /* number of primes stored   */
    static idx_t next_k = 0;         /* next k to test           */

    /* Grow the cache if needed. */
    if (primes == NULL) {
        capacity = 64;
        primes = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!primes) exit(1); /* allocation failed */
    }

    /* Generate primes until we have at least n of them. */
    while (size < n) {
        /* candidate = 45*k + 29   (all arithmetic in idx_t to avoid overflow). */
        idx_t prod = 45 * next_k;
        seq_t candidate = (seq_t)(prod + 29ULL);

        if (is_prime_big(candidate)) {
            /* Ensure capacity */
            if (size == capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, capacity * sizeof(seq_t));
                if (!tmp) exit(1); /* allocation failed */
                primes = tmp;
            }
            primes[size++] = candidate;
        }
        next_k++;
    }

    return primes[n - 1];
}
