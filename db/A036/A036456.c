/*
 * A036456  Numbers k for which exactly 4 applications of A000005 are needed
 *          to reach 2.
 * https://oeis.org/A036456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate integers, compute d(d(d(n))) via factorization;
   accept a number if the third iterate equals 3 or 5 (the only odd
   primes >2 that can occur).  The nth such number is the desired
   term. */
 
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return the number of positive divisors of n using factor_u64. */
static uint32_t divisor_count_u64(uint64_t n)
{
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint32_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
        res *= (uint32_t)(exps[i] + 1);
    return res;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t x = 1;
    while (count < n) {
        uint32_t d1 = divisor_count_u64((uint64_t)x);
        uint32_t d2 = divisor_count_u64((uint32_t)d1);
        uint32_t d3 = divisor_count_u64((uint32_t)d2);
        if (d3 == 3 || d3 == 5) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
    /* Should never be reached */
    return 0;
}
