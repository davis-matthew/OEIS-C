/*
 * A142758  Primes congruent to 31 mod 59.
 * https://oeis.org/A142758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 111, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that p ≡ 31 (mod 59). */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Sequence starts at 31.  We step through numbers congruent to 31
       mod 59 and test each for primality. */
    if (n < 1) return (seq_t)0; /* undefined, but safe guard */

    seq_t cur = 31;          /* first candidate */
    idx_t  rank = 1;         /* current prime rank */

    while (rank < n)
    {
        cur += 59;                   /* next number ≡ 31 (mod 59) */
        if (is_prime_big(cur))       /* test primality */
            rank++;                  /* found a prime, increment rank */
    }

    return cur;                      /* nth prime in the progression */
}
