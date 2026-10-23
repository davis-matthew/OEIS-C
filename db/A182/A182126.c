/*
 * A182126  a(n) = prime(n)*prime(n+1) mod prime(n+2).
 * https://oeis.org/A182126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)*prime(n+1) mod prime(n+2)
 * Range:     u8 n <= 52, u16 n <= 4888, u32 n <= 7017, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 1);
    seq_t p3 = (seq_t)get_prime(n + 2);
    return (p1 * p2) % p3;
}
