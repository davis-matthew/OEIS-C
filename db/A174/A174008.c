/*
 * A174008  a(n) = n-th prime plus n-th even nonnegative nonprime.
 * https://oeis.org/A174008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A000040(n) + A163300(n) = prime(n) + (n==1?0:2n)
 * Range:     u8 n <= 40, u16 n <= 5541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t prime_val = (seq_t)get_prime(n);
    seq_t even_nonprime = (n == (idx_t)1) ? (seq_t)0 : ((seq_t)2 * (seq_t)n);
    return prime_val + even_nonprime;
}
