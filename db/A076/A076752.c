/*
 * A076752  a(n) = Sum_{d is a square divisor of n} n/d.
 * https://oeis.org/A076752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = Sum_{d is a square divisor of n} n/d
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factor n into prime powers */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* multiplicative accumulation */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t term = 1;
        seq_t p = (seq_t)primes[i];
        size_t e = (size_t)exps[i];
        /* recurrence:
           a(p^(m+1)) = p * a(p^m)      for even m
                      = p * a(p^m) + 1 for odd  m            */
        for (size_t m = 0; m < e; ++m)
        {
            if ((m & 1) == 0)
                term = term * p;
            else
                term = term * p + 1;
        }
        result = result * term;
    }
    return result;
}
