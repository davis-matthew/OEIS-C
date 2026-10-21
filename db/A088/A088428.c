/*
 * A088428  Number of primes in arithmetic progression starting with 29 and
 *          with d=2n.
 * https://oeis.org/A088428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of consecutive primes in the arithmetic progression
   starting at 29 with common difference d = 2*n, stopping when the first
   composite appears. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Common difference 2*n */
    idx_t step = 2 * n;

    /* Current term of the progression (uint64_t because is_prime expects idx_t) */
    idx_t cur  = 29;

    seq_t count = 0;

    while (is_prime(cur))
    {
        ++count;
        cur += step;
    }

    return count;
}
