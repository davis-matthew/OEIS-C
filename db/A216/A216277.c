/*
 * A216277  Primes which cannot be written as x^2 + 5*y^2, where x >= 0, y >=
 *          0.
 * https://oeis.org/A216277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that p mod 20 ∉ {1,5,9}
 * Range:     u8 n <= 43, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all primes that are NOT congruent to 1, 5, or 9 modulo 20. */
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1; undefined for n==0 */
        return (seq_t)0;            /* sentinel value – not used for valid calls */

    idx_t count = 0;                /* how many qualifying primes found so far */

    /* Handle the prime 2 separately – it satisfies the condition */
    if ( ++count == n )
        return (seq_t)2;

    /* Now iterate through odd candidates only */
    for ( idx_t cand = 3; ; cand += 2 ) {
        if (!is_prime(cand))
            continue;

        unsigned r = (unsigned)(cand % 20ULL);     /* compute p mod 20 correctly */
        if (r == 1u || r == 5u || r == 9u)
            continue;                          /* skip primes with forbidden residues */

        if ( ++count == n )                    /* found the nth required prime */
            return (seq_t)cand;
    }
}
