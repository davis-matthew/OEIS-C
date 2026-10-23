/*
 * A130796  Primes p such that nextprime(p)-p is not power of 2.
 * https://oeis.org/A130796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_power_of_two, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A130796: primes p such that nextprime(p)-p is not a power of 2 */
/* The function searches for primes in ascending order, storing those
   whose gap to the next prime is not a power of two.  Results are
   cached so that calls with arbitrary indices remain efficient. */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_power_of_two.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    /* Guard for a zero index (not part of the OEIS sequence) */
    if (n == 0) return 0;

    /* static cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* current candidate prime for the next search */
    static idx_t cur_p = 2;

    while (cache_len < n) {
        /* Ensure cur_p is prime */
        if (!is_prime(cur_p)) {
            cur_p = next_prime(cur_p + 1);
        }

        /* Find the next prime after cur_p */
        idx_t q = next_prime(cur_p);   /* nextprime(cur_p) */

        seq_t diff = (seq_t)(q - cur_p);
        if (!is_power_of_two(diff)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur_p;
        }

        /* Advance to the prime after cur_p */
        cur_p = q;
    }

    return cache[n - 1];
}
