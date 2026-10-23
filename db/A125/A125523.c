/*
 * A125523  Democratic primes: primes such that the left half of the prime is
 *          prime and the right half is not.
 * https://oeis.org/A125523
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth Democratic prime (a(2)=29)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;                 /* undefined below the offset */

    /* Static cache of Democratic primes */
    static seq_t *cache        = NULL;  /* array of found primes          */
    static size_t cache_used   = 0;     /* number of primes stored        */
    static size_t cache_cap    = 0;     /* allocated capacity             */
    static idx_t  next_candidate = 2;   /* next integer to test           */

    /* Generate primes until we have (n‑1) Democratic primes
       (since a(2) is the first term). */
    while (cache_used < (size_t)(n - 1)) {
        /* Resize cache if necessary */
        if (cache_used >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;          /* allocation failure - unlikely   */
            cache    = tmp;
            cache_cap = new_cap;
        }

        /* Test the current candidate for primality */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t p   = (seq_t)next_candidate;
            int   len = digit_len(p);
            int   half = len / 2;

            /* Left half: first floor(len/2) digits */
            seq_t left_den = pow10((idx_t)(len - half));
            seq_t left     = p / left_den;

            /* Right half: last floor(len/2) digits */
            seq_t right    = p % pow10((idx_t)half);

            if (is_prime_big(left) && !is_prime_big(right)) {
                cache[cache_used++] = p;
            }
        }
        ++next_candidate;
    }

    return cache[n - 2];
}
