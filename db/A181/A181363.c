/*
 * A181363  1 followed by the primes, interleaved recursively.
 * https://oeis.org/A181363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(2n)=a(n), a(2n+1)=prime(n) (index of prime)
 * Range:     u8 n <= 110, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* base case */
    if (n == 1)
        return (seq_t)1;

    /* even indices: recurse halving */
    if ((n & 1) == 0)
        return a(n >> 1);

    /* odd indices >1: return the (n-1)/2-th prime */
    idx_t k = (n - 1) >> 1;       /* k >= 1 for n>1 */
    return (seq_t) get_prime(k);
}
