/*
 * A230230  Number of ways to write 2*n = p + q with p, q, 3*p - 10, 3*q + 10
 *          all prime.
 * https://oeis.org/A230230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine computes the OEIS sequence A230230.
   For a given n > 0 it counts the number of ordered pairs of primes
   (p, q) such that                2*n = p + q
                                   3*p - 10 is prime
                                   3*q + 10 is prime.
   The routine builds a list of all primes up to 3*(2*n)+10, then
   iterates through that list testing each candidate p.  It counts a
   pair when all required numbers are prime.  The routine is
   portable to the grad‑ed types seq_t (the sequence value) and
   idx_t (the index, which is unsigned 64‑bit).  No printing or file
   I/O is performed.  Runtime is O(number_of_primes_up_to_3*2*n),
   which for n <= 10 000 is far below the time limit. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1); for n < 1 the definition is irrelevant */
    idx_t twon = 2 * n;

    /* The largest number we need to test for primality is at most
       max(3*twon + 10, twon).  We choose the larger bound.  Although
       3*twon + 10 is always ≥ twon for n >= 1, the former expression
       is kept for clarity. */
    idx_t max_needed = 3 * twon + 10;

    /* Build the list of primes up to max_needed.  The routine
       allocates an array of uint64_t which we must free after use. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(max_needed, &primes, &prime_cnt);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* If p >= 2*n then q = 2*n - p <= 0, not prime */
        if (p >= twon)
            break;

        uint64_t q = twon - p;
        if (q < 2)          /* q must be at least the first prime 2 */
            continue;

        /* q must be prime */
        if (!is_prime((idx_t)q))
            continue;

        /* 3*p - 10 must be prime; it is < 2 only when p < 4 */
        if (p < 4)
            continue;           /* 3*p - 10 would be <= 2, not prime */
        uint64_t v1 = 3 * p - 10;
        if (!is_prime((idx_t)v1))
            continue;

        /* 3*q + 10 is always >= 16 for q >= 2, so just test primality */
        uint64_t v2 = 3 * q + 10;
        if (!is_prime((idx_t)v2))
            continue;

        /* All required numbers are prime: count this ordering. */
        ++count;
    }

    free(primes);
    return count;
}
