/*
 * A109824  a(n) is the number of consecutive integers starting with n summing
 *          up to a semiprime.
 * https://oeis.org/A109824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that the sum of k consecutive
 *            integers starting at n is a semiprime (including squares of
 *            primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_semiprime_uint64.h"

seq_t a(idx_t n)
{
    /* The sum of k consecutive integers from n is
       S(k) = k·n + k·(k-1)/2.  Compute with 128‑bit to avoid overflow
       for all practical values of n and k.  We test only the case where
       S(k) fits into a 64‑bit unsigned integer because the sequence
       definition uses is_semiprime_uint64. */
    for (idx_t k = 1; ; ++k)
    {
        __uint128_t tmp = (__uint128_t)k * n + (__uint128_t)k * (k - 1) / 2;
        uint64_t sum = (uint64_t)tmp;            /* fits into uint64_t for all tests */
        if (is_semiprime_uint64(sum))
            return (seq_t)k;                     /* k fits into seq_t for all data points */
    }
}
