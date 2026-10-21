/*
 * A088425  Number of primes in arithmetic progression starting with 17 and
 *          with d=2n.
 * https://oeis.org/A088425
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

/* Computes A088425: the number of primes in the arithmetic progression
   17, 17+2n, 17+2n*2, … until the first composite term.  The routine
   simply iterates the progression and counts primes. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Step size for the progression: 2n */
    idx_t diff = 2 * n;

    /* First term of the progression */
    idx_t cur = 17;

    /* Counter of consecutive primes */
    idx_t count = 0;

    /* Loop until the first composite term is reached */
    while (is_prime(cur))
    {
        ++count;
        cur += diff;
    }

    return (seq_t)count;
}
