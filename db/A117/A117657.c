/*
 * A117657  Number of solutions to x^(k+3)=x^3 mod n for some k>=1.
 * https://oeis.org/A117657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(p^e) = p^⌊2e/3⌋ + (p-1)*p^(e-1), multiplicative
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of solutions to x^(k+3)=x^3 mod n for some k≥1 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;           /* a(1) = 1 by definition */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        unsigned int t1 = (unsigned int)((2ULL * e) / 3ULL);   /* floor(2e/3) */
        seq_t term1 = pow_seq((seq_t)p, (int)t1);

        seq_t term2 = (seq_t)0;
        if (e > 0)
        {
            seq_t pow_pe_1 = pow_seq((seq_t)p, (int)(e - 1));
            term2 = seq_mul(pow_pe_1, (seq_t)(p - 1));
        }

        seq_t val = term1 + term2;
        result = seq_mul(result, val);
    }

    return result;
}
