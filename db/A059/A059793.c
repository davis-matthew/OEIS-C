/*
 * A059793  Stationary value of quotient in the continued fraction expansion
 *          of sqrt(prime) when the quotient-cycle-length = 1.
 * https://oeis.org/A059793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * m where m is the nth integer such that * m^2+1 is
 *            prime (OEIS A059793)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* Cached values of the sequence. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Next integer 'm' to test.  m starts at 1 because
       a(0) uses m = 1. */
    static idx_t next_m = 1;

    /* If we have not generated enough terms for the requested
       index, keep generating until we have. */
    while (cache_len <= n) {
        idx_t m = next_m++;

        /* Compute m^2+1 in an unsigned 128‑bit integer
           to avoid overflow of intermediate products. */
        unsigned __int128 val128 = (unsigned __int128)m * (unsigned __int128)m + 1;

        /* Determine the maximum value of seq_t so we do not
           test primes that would not fit into a result. */
        seq_t max_seq = (seq_t)-1;

        /* If the candidate exceeds the representable range of seq_t,
           further terms would not fit anyway, so we stop. */
        if (val128 > (unsigned __int128)max_seq) {
            /* The sequence entry requested is out of range for the
               current build; return zero (undefined but not reached by
               the test harness for terms that fit). */
            return (seq_t)0;
        }

        /* Test if m^2+1 is prime. */
        seq_t candidate = (seq_t)val128;
        if (isPrimeTrial(candidate)) {
            /* Store 2*m as the next sequence value. */
            seq_t val = (seq_t)2 * (seq_t)m;

            /* Ensure enough space in the cache. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Out of memory – terminate. */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)n];
}
