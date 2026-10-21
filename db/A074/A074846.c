/*
 * A074846  a(n) is the smallest k > n such that n*k+1 is prime.
 * https://oeis.org/A074846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > n such that n*k + 1 is prime
 * Range:     u8 n <= 12, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Computes a(n) for OEIS sequence A074846.
 *
 *   a(n) = min{ k > n : n*k + 1 is prime }.
 *
 * The function uses trial increments starting at k = n+1 and tests
 * n*k+1 for primality with the provided is_prime_big() routine.
 * For 64‑bit builds the product is computed in 128‑bit to avoid
 * overflow before being cast to seq_t.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t for multiplication */
    for (idx_t k = n + 1; ; ++k)
    {
        /* compute n*k + 1 using 128‑bit intermediate to avoid overflow
           for 64‑bit seq_t builds. */
        __uint128_t tmp = (__uint128_t)sn * k + 1;
        seq_t prod = (seq_t)tmp;
        if (is_prime_big(prod))
            return (seq_t)k;     /* returned k fits in seq_t */
    }
}
