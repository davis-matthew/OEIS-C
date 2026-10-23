/*
 * A035531  a(n) = A000120(n) + A001221(n) - 1.
 * https://oeis.org/A035531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = popcount(n) + distinct_prime_factors(n) - 1
 * Range:     u8 n <= 9113, u16 n <= 9113, u32 n <= 9113, u64 n <= 9113,
 *            u128 n <= 9113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000120(n) is the binary weight (number of 1-bits),
   A001221(n) is the number of distinct prime divisors. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*---------------------------------------------------------------------------*/
    /* Count binary 1s (popcount) with the classic Kernighan algorithm.           */
    seq_t pop = 0;
    idx_t m = n;
    while (m) {
        pop++;
        m &= m - 1;
    }

    /*---------------------------------------------------------------------------*/
    /* Count distinct prime divisors.  For n <= 2^64 a trial division loop is
       fast enough for the expected input range (n up to ~10k for full sequence
       coverage).  No overflow occurs because the loop uses the bound
       i <= temp / i. */
    seq_t primes = 0;
    idx_t temp = n;

    if ((temp & 1) == 0) {          /* factor 2 */
        primes++;
        while ((temp & 1) == 0)
            temp >>= 1;
    }

    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            primes++;
            while (temp % i == 0)
                temp /= i;
        }
    }
    if (temp > 1)  /* remaining prime factor */
        primes++;

    /*---------------------------------------------------------------------------*/
    return pop + primes - 1;
}
