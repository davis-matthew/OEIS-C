/*
 * A011974  2 followed by the numbers that are the sum of 2 successive primes.
 * https://oeis.org/A011974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2; for n≥2, a(n)=prime(n-1)+prime(n) where prime(k) is the
 *            * k-th prime (1‑based)
 * Range:     u8 n <= 31, u16 n <= 3513, u32 n <= 9953, u64 n <= 9953,
 *            u128 n <= 9953, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    seq_t p1 = (seq_t)get_prime(n - 1);
    seq_t p2 = (seq_t)get_prime(n);
    return p1 + p2;
}
