/*
 * A176345  Sum of gcd(k,n) from k = 1 to n over "regular" integers only (an
 *          integer k is regular if there is an x such that k^2 x == k (mod
 *          n)).
 * https://oeis.org/A176345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} ( 2·p^e – p^(e‑1) ) (OEIS A176345)
 * Range:     u8 n <= 59, u16 n <= 4289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only standard arithmetic on seq_t and a small prime‑factor routine. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* prototype: void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* By convention a(1) = 1 – the empty product */
    if (n == 1)
        return (seq_t)1;

    /* Factor n into primes and exponents.  At most 64 distinct primes can divide a 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    for (size_t idx = 0; idx < pcnt; ++idx)
    {
        seq_t p  = (seq_t)primes[idx];
        uint64_t e = exps[idx];

        /* Compute p^e and p^(e‑1) using normal multiplication on seq_t. */
        seq_t pe  = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j)
            pe *= p;

        seq_t pe1 = (seq_t)1;
        if (e > 1)
        {
            for (uint64_t j = 0; j < e - 1; ++j)
                pe1 *= p;
        }

        /* term = 2·p^e – p^(e‑1) */
        seq_t term = (seq_t)2 * pe - pe1;

        result *= term;
    }

    return result;
}
