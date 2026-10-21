/*
 * A066108  Sum n^d over all divisors of n.
 * https://oeis.org/A066108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} n^d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n=0; return 0 for safety */

    /* factorise n   (n is small, trial division is fine) */
    uint64_t primes[10];
    uint64_t exps[10];
    size_t pcnt = 0;

    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            primes[pcnt]   = p;
            exps[pcnt]     = e;
            ++pcnt;
        }
    }
    if (m > 1)
    {
        primes[pcnt]   = m;
        exps[pcnt]     = 1;
        ++pcnt;
    }

    /* enumerate all divisors of n */
    idx_t divs[128];
    size_t divc = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        idx_t tmp[128];
        size_t tmpc = 0;

        for (size_t j = 0; j < divc; ++j)
        {
            idx_t base = divs[j];
            idx_t val  = 1;

            for (uint64_t k = 0; k <= e; ++k)
            {
                tmp[tmpc++] = (idx_t)(base * val);
                val *= p;
            }
        }
        memcpy(divs, tmp, tmpc * sizeof(idx_t));
        divc = tmpc;
    }

    /* sum n^d over all divisors d */
    seq_t sum = 0;
    for (size_t i = 0; i < divc; ++i)
    {
        seq_t term = big_pow((seq_t)n, (seq_t)divs[i]);
        sum += term;
    }
    return sum;
}
