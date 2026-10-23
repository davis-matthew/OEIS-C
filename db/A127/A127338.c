/*
 * A127338  Numbers that are the sum of 11 consecutive primes.
 * https://oeis.org/A127338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + p_{n+1} + ... + p_{n+10}
 * Range:     u8 n <= 3, u16 n <= 775, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where p_k is the k-th prime (1‑indexed). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1 is the official offset; behaviour for n < 1 is undefined. */
    seq_t sum = 0;
    for (idx_t i = 0; i < 11; ++i)
        sum += (seq_t)get_prime(n + i);
    return sum;
}
