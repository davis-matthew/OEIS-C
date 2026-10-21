/*
 * A237261  Number of ways to write the n-th odd number in the form of
 *          2^k+p^m*q^h, where p < q are 1 or odd prime numbers, and k, m, h
 *          >= 1.
 * https://oeis.org/A237261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ways to write the n‑th odd number 2*n-1 as 2^k
 *            + p^m * q^h, with k,m,h ≥ 1, p<q, p can be 1 (with m forced to
 *            1); p and q are odd primes
 * Range:     u8 n <= 698, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* offset starts at 1; for n == 0 return 0 */
    if (n == 0)
        return 0;

    seq_t odd   = (seq_t)n * 2ULL - 1ULL;   /* the nth odd integer */
    seq_t count = 0;

    seq_t pow2 = 2ULL;            /* 2^1 */
    while (pow2 < odd)
    {
        seq_t T = odd - pow2;
        if (T > 1ULL)            /* T must be odd ≥ 3 to allow a representation */
        {
            seq_t rem        = T;
            int    distinct  = 0;

            /* factorization by trial division using odd divisors only */
            for (idx_t p = 3; (seq_t)p * (seq_t)p <= rem; p += 2)
            {
                if (rem % (seq_t)p == 0)
                {
                    distinct++;
                    while (rem % (seq_t)p == 0)
                        rem /= (seq_t)p;
                    if (distinct > 2)   /* more than two primes – discard */
                        break;
                }
            }
            if (distinct <= 2 && rem > 1ULL)  /* remaining prime factor > sqrt(rem) */
                distinct++;

            /* a representation exists if the factorization has 1 or 2 distinct primes */
            if (distinct == 1 || distinct == 2)
                count++;
        }

        /* advance to next power of two, avoiding overflow */
        if (pow2 > odd / 2ULL)
            break;
        pow2 <<= 1;
    }

    return count;
}
