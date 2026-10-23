/*
 * A133549  Sum of the fourth powers of the first n odd primes.
 * https://oeis.org/A133549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the fourth powers of the first n odd primes
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* Off‑set starts at 1; behavior for 0 is undefined */

    seq_t sum = 0;
    idx_t limit = n + 1;                    /* odd primes are primes 2.. */

    for (idx_t k = 2; k <= limit; ++k)      /* primes[2] = 3, primes[3] = 5, ... */
    {
        seq_t p  = (seq_t)get_prime(k);
        seq_t p2 = p * p;                   /* p² */
        sum += p2 * p2;                     /* p⁴ */
    }

    return sum;
}
