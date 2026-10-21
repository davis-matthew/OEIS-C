/*
 * A085957  Primes p such that 6p + 1 and (p-1)/6 are primes.
 * https://oeis.org/A085957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 22, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085957: primes p such that 6p+1 and (p-1)/6 are primes. */
/* The function returns the nth term (n ≥ 1).  The implementation
   simply scans candidates p ≡ 1 (mod 6) and tests the primality of
   p, (p-1)/6 and 6p+1 using is_prime_big.  Complexity is
   roughly O(n log n) and easily satisfies the 2‑second limit for
   n up to ≈ 10000 on a normal machine. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1 – behaviour below is irrelevant */
    seq_t count = 0;
    seq_t p = 13;                         /* first prime ≡ 1 (mod 6) in the sequence */

    while (true)
    {
        /* Check that p is prime first */
        if (is_prime_big(p))
        {
            seq_t r = (p - 1) / 6;        /* (p-1)/6 must be prime */
            seq_t q = 6 * p + 1;          /* 6p+1 must be prime */

            if (is_prime_big(r) && is_prime_big(q))
            {
                ++count;
                if (count == (seq_t)n)
                    return p;            /* found the nth term */
            }
        }

        p += 6;                          /* next candidate ≡ 1 (mod 6) */
    }
}
