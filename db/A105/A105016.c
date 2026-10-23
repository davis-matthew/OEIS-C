/*
 * A105016  Smallest a(n) such that a(n)^2 - n is a positive prime, or 0 if no
 *          such a(n) exists.
 * https://oeis.org/A105016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A105016: a(n) = smallest a>0 such that a^2 - n is a positive prime; 
   a(n)=0 if no such a exists.
   The function computes a(n) by checking increasing a and testing if
   a^2 - n is prime.  It stops when the difference exceeds a preset
   limit, assuming that for n ≤ 10 000 all solutions fall below that
   limit (empirical for the sequence).
   The limit is 10 000 000, which guarantees at most ~3 200 iterations
   per call (since a^2 grows quadratically).
   Primality testing uses the supplied is_prime function.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

#define PRIME_LIMIT 10000000ULL   /* 1e7 threshold for the difference a^2 - n */

seq_t a(idx_t n)
{
    idx_t n_val = n;                 /* copy into local idx_t for clarity */
    idx_t root   = int_sqrt(n_val);   /* floor(sqrt(n)) */
    seq_t a_start = (seq_t)root + 1;  /* smallest a with a^2 > n */
    seq_t a_val   = a_start;

    while (true)
    {
        /* compute difference a^2 - n; safe in 64-bit for our limits */
        uint64_t diff = (uint64_t)a_val * (uint64_t)a_val - (uint64_t)n_val;

        if (diff <= 1ULL)
        {
            /* diff must be a positive prime, 1 is not prime */
            ;
        }
        else if (diff > PRIME_LIMIT)
        {
            /* decision point: assume no primes beyond this difference
               for all n <= 10 000 (empirical). */
            return 0U;
        }
        else
        {
            /* test primality using the utility function */
            if (is_prime(diff))
                return a_val;   /* found minimal a */
        }

        /* next a */
        a_val = (seq_t)((uint64_t)a_val + 1ULL);
    }
}
