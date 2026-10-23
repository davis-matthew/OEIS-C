/*
 * A161010  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 14.
 * https://oeis.org/A161010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} μ(n/d)*d^(14-1)/φ(n) equivalent multiplicative
 *            form a(p^e) = p^(12e-12) * (p^13-1)/(p-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 37, u128 n <= 1469,
 *            big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Offset: a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    /* Factorise n.  The array sizes are ample for a 64‑bit number. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t term;
        if (sizeof(seq_t) <= 8) {            /* 64‑bit build – use __int128 for safety */
            unsigned __int128 term1 = 1;
            uint64_t exp1 = 12 * e - 12;     /* exponent for p^(12e-12) */
            for (uint64_t k = 0; k < exp1; ++k)
                term1 *= (unsigned __int128)p;

            unsigned __int128 numerator = 1;
            for (uint64_t k = 0; k < 13; ++k)
                numerator *= (unsigned __int128)p;
            numerator -= 1;                  /* p^13 - 1 */

            unsigned __int128 denom = (unsigned __int128)(p - 1);
            unsigned __int128 term2 = numerator / denom;

            unsigned __int128 prod = term1 * term2;
            term = (seq_t)prod;
        } else {                            /* arbitrary‑precision build */
            seq_t term1 = seq_pow((seq_t)p, 12 * e - 12);
            seq_t p13   = seq_pow((seq_t)p, 13);
            seq_t term2 = (p13 - 1) / ((seq_t)p - 1);
            term = seq_mul(term1, term2);
        }

        if (sizeof(seq_t) <= 8)
            result = (seq_t)((unsigned __int128)result * term);
        else
            result = seq_mul(result, term);
    }

    return result;
}
