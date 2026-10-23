/*
 * A141346  Sum of the distinct digits of the prime factors of n.
 * https://oeis.org/A141346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of the distinct digits of the prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorize.h"

seq_t a(idx_t n)
{
    /* a(1) = 0; behaviour for n < 1 is unspecified */
    if (n < 2)
        return (seq_t)0;

    /* Factor n into distinct primes with factorize */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize((uint64_t)n, primes, exps);

    /* Compute mask of digits seen in all distinct prime factors */
    uint16_t mask = 0;          /* 10‑bit mask for digits 0–9 */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        while (p > 0) {
            uint8_t d = (uint8_t)(p % 10);
            mask |= (uint16_t)(1U << d);
            p /= 10;
        }
    }

    /* Sum the digits represented in the mask */
    seq_t sum = 0;
    for (int d = 0; d < 10; ++d) {
        if (mask & (1U << d))
            sum += (seq_t)d;
    }
    return sum;
}
