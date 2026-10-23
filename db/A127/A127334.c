/*
 * A127334  Numbers that are the sum of 7 consecutive primes.
 * https://oeis.org/A127334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n)+p(n+1)+p(n+2)+p(n+3)+p(n+4)+p(n+5)+p(n+6) where p(k)
 *            is the k‑th prime (p(1)=2). This returns the sum of 7
 *            consecutive primes, starting with the n‑th prime
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i < 7; ++i)
        sum += (seq_t)get_prime(n + i);
    return sum;
}
