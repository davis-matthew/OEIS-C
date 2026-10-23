/*
 * A123128  Add n to the n-th difference between consecutive primes.
 * https://oeis.org/A123128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (prime(n+1) - prime(n))
 * Range:     u8 n <= 247, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t pn  = get_prime(n);
    seq_t pn1 = get_prime(n + 1);
    return (seq_t)n + (pn1 - pn);
}
