/*
 * A023594  a(n) = sum of distinct prime divisors of 2*prime(n)+1.
 * https://oeis.org/A023594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of 2*prime(n)+1
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* obtain the n-th prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* compute m = 2*p + 1 as seq_t */
    seq_t m = (seq_t)2 * (seq_t)p + (seq_t)1;

    /* factor m and sum its distinct prime factors */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];
    return sum;
}
