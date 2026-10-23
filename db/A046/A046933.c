/*
 * A046933  Number of composites between successive primes.
 * https://oeis.org/A046933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = prime(n+1) – prime(n) – 1 (number of composites between
 *            successive primes, OEIS A046933)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1; behaviour for n < 1 is irrelevant. */
    idx_t p1 = (idx_t)get_prime(n);
    idx_t p2 = (idx_t)get_prime(n + 1);
    idx_t diff = p2 - p1 - 1;
    return (seq_t)diff;
}
