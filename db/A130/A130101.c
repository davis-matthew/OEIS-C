/*
 * A130101  Primes prime(k) such that 10*prime(k) + prime(k+1) is not prime.
 * https://oeis.org/A130101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130101: The nth prime p such that 10p + nextprime(p) is composite.
   The function lazily generates primes using next_prime() and caches
   the qualifying ones. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state retained across calls. */
    static uint64_t last_checked = 1;   /* start before the first prime */
    static size_t count = 0;            /* number of stored results      */
    static seq_t *cache = NULL;         /* array of qualifying primes    */
    static size_t cap = 0;              /* allocated capacity of cache   */

    /* Ensure we have at least n results. */
    while (count < (size_t)n) {
        /* Get next prime p. */
        last_checked = next_prime(last_checked);
        uint64_t p = last_checked;
        /* Find the next prime after p. */
        uint64_t q = next_prime(p);

        /* Compute 10*p + q as seq_t (enough to hold the value). */
        seq_t combo = (seq_t)p * 10 + (seq_t)q;

        /* Store p if the test fails (i.e., combo is composite). */
        if (!is_prime_big(combo)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    abort();                 /* unrecoverable allocation error */
                }
                cache = tmp;
            }
            cache[count++] = (seq_t)p;
        }
    }

    /* Return the nth element (offset 1). */
    return cache[count - 1];
}
