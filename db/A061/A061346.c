/*
 * A061346  Odd numbers that are neither primes nor prime powers.
 * https://oeis.org/A061346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th odd composite number that is NOT a prime power.
 * Computation: a linear search over odd integers, testing each candidate
 * for compositeness and for having at least two distinct prime factors.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term listed in the OEIS entry. */
    if (n == 0)          /* defensive guard – not part of the official definition */
        return 0;

    idx_t count = 0;      /* number of acceptable terms found so far */
    idx_t x = 3;          /* first odd number > 1 */

    for (;;)
    {
        /* Find the smallest odd divisor of x.  If none is found then x is prime. */
        idx_t spf = 0;    /* smallest prime factor of x (0 if x is prime) */
        for (idx_t d = 3; d <= x / d; d += 2)
        {
            if (x % d == 0)
            {
                spf = d;
                break;
            }
        }

        if (spf == 0)
        {
            /* x is prime – skip */
        }
        else
        {
            /* x is composite.  Determine whether it is a prime power. */
            idx_t rem = x;
            while (rem % spf == 0)
                rem /= spf;
            if (rem != 1)        /* at least two distinct prime factors */
            {
                ++count;
                if (count == n)
                    return (seq_t)x;   /* found the nth term */
            }
        }

        /* Next odd integer */
        x += 2;
    }
}
