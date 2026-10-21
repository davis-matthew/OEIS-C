/*
 * A087191  Primes of the form k^4 - (k+1)^3.
 * https://oeis.org/A087191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 61, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087191: primes of the form k^4 – (k+1)^3
   Offset 1 – a(1) corresponds to k = 3.  Each term is tested for
   primality before being cached so that calls for arbitrary n
   (in any order) are answered in O(1) after the first expansion. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* cache of found primes, growing as needed */
    static seq_t *cache   = NULL;
    static idx_t  len     = 0;   /* number of primes stored   */
    static idx_t  cap     = 0;   /* current allocation size   */
    static idx_t  next_k  = 3;   /* next k value to try        */

    /* expand the cache if we need more space */
    if (n > cap) {
        idx_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* generate primes until we have at least n of them */
    while (len < n) {
        seq_t val;
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* 64‑bit seq_t – use 128‑bit intermediates to avoid overflow */
            unsigned __int128 k  = (unsigned __int128)next_k;
            unsigned __int128 k4 = k * k * k * k;
            unsigned __int128 kp1= k + 1;
            unsigned __int128 kp13= kp1 * kp1 * kp1;
            unsigned __int128 val128 = k4 - kp13;

            if (val128 > (unsigned __int128)UINT64_MAX) {
                /* we cannot represent larger values in a 64‑bit type;
                   subsequent terms would also be too large, so stop. */
                break;
            }
            val = (seq_t)val128;
        } else {
            /* seq_t is larger than 64‑bit – use seq_t arithmetic
               which is defined for the arbitrary‑precision type. */
            seq_t k   = (seq_t)next_k;
            seq_t k4  = k * k * k * k;
            seq_t kp1 = k + 1;
            seq_t kp13= kp1 * kp1 * kp1;
            val = k4 - kp13;
        }

        if (isPrimeGeneric(val)) {
            cache[len++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
