/*
 * A127064  a(0)=1. a(n) = a(prime(n)(mod n)) + 1, where prime(n) is the n-th
 *          prime.
 * https://oeis.org/A127064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(prime(n) mod n) + 1 with a(0) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    } else {
        idx_t m = (idx_t)get_prime(n) % n;
        return a(m) + (seq_t)1;
    }
}
