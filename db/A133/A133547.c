/*
 * A133547  a(n) = sum of squares of first n odd primes.
 * https://oeis.org/A133547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{i=1}^n p_{i+1}^2 where p_k is the k-th prime. The
 *            odd primes are the primes with index ≥ 2
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, but return 0 safely */

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        uint32_t p = get_prime(i + 1);      /* odd primes start at index 2 */
        sum += (seq_t)p * p;
    }
    return sum;
}
