/*
 * A156009  Primes p such that 64*p + 189 is also prime.
 * https://oeis.org/A156009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 64*p + 189 is prime
 * Range:     u8 n <= 1, u16 n <= 47, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n);       */

seq_t a(idx_t n)
{
    /* undefined for n < 1; return 0 for safety */
    if (n == 0) return 0;

    idx_t count = 0;   /* how many qualifying primes found  */
    idx_t p    = 0;    /* current prime candidate          */

    while (count < n)            /* loop until the nth candidate found */
    {
        /* get the next prime */
        p = next_prime(p + 1);

        /* compute 64*p + 189 in seq_t */
        seq_t target = (seq_t)64 * (seq_t)p + 189;

        /* test if the associated number is prime */
        if (is_prime_big(target))
            ++count;          /* this p qualifies */
    }

    /* p now holds the nth prime satisfying the condition */
    return (seq_t)p;
}
