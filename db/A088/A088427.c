/*
 * A088427  Number of primes in arithmetic progression starting with 23 and
 *          with d=2n.
 * https://oeis.org/A088427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of primes in the arithmetic progression
   23, 23+2n, 23+4n, … until the first composite is encountered. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For the offset handling; behaviour for n==0 is irrelevant. */
    if (n == 0) return 0;

    seq_t d = (seq_t)2 * (seq_t)n;   /* common difference */
    seq_t count = 0;

    for (idx_t k = 0; ; ++k)
    {
        seq_t term = (seq_t)23 + d * (seq_t)k;
        if (is_prime_big(term))
            ++count;
        else
            break;
    }

    return count;
}
