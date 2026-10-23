/*
 * A162295  Primes of the form k^3-k^2-k-1.
 * https://oeis.org/A162295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k^3 - k^2 - k - 1 where k is the n‑th integer for which *
 *            the expression is prime. * The function searches k in increasing
 *            order, checking each value with * the primality test from utils.
 *            Results are cached so that the sequence * can be queried in any
 *            order
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 166, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache of found primes and the next k to test */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t size    = 0;
    static uint64_t next_k = 4;  /* first k that yields a prime (a(1)=43) */

    /* guard against zero index (should not happen in the specification) */
    if (n == 0)
        return (seq_t)0;

    /* generate primes until we have at least n terms */
    while (size < n) {
        seq_t k   = (seq_t)next_k;
        seq_t k2  = k * k;
        seq_t val = k * k2 - k2 - k - 1;  /* k^3 - k^2 - k - 1 */

        if (isPrimeGeneric(val)) {
            /* grow cache if necessary */
            if (cache == NULL) {
                cap   = 4;
                cache = (seq_t *)malloc(sizeof(seq_t) * cap);
            } else if (size >= cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            }
            cache[size++] = val;
        }
        next_k++;
    }
    return cache[n - 1];
}
