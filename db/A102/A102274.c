/*
 * A102274  Primes p such that Q(sqrt(-21p)) has genus characters chi_{-3} =
 *          -1, chi_{-7} = -1.
 * https://oeis.org/A102274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p ≡ 47, 59, 83 (mod 84) (i.e. primes p where
 *            (-3/p)=(-7/p)=-1)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Naïve incremental search with caching between calls */
    static uint64_t last_candidate = 1; /* start before the first prime candidate */
    static idx_t  primes_found = 0;

    while (primes_found < n)
    {
        last_candidate++;                                        /* next candidate */

        /* Fast modular test for the three residue classes */
        uint64_t r = last_candidate % 84ULL;
        if (r != 47ULL && r != 59ULL && r != 83ULL)
            continue;                                            /* not in the required classes */

        /* Primality test – the util function is efficient enough for the range we need */
        if (is_prime(last_candidate))
        {
            primes_found++;                                       /* a new qualifying prime */
        }
    }
    return (seq_t)last_candidate;                                 /* the nth prime satisfying the condition */
}
