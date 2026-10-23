/*
 * A046913  Sum of divisors of n not congruent to 0 mod 3.
 * https://oeis.org/A046913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   a(n) = sigma(3*n) - 3 * sigma(n) where sigma is the sum of
 *            divisors function. Computation is done by factorising n,
 *            deriving sigma(n) and sigma(3*n) from the prime factors, and
 *            applying the multiplicative formula for sigma
 * Range:     u8 n <= 16, u16 n <= 1696, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;           /* undefined for 0, safe guard */

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma_n   = (seq_t)1;
    seq_t sigma_3n  = (seq_t)1;
    int    exp3     = 0;            /* exponent of prime 3 in n (if any) */
    int    has3     = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int      e = (int)exps[i];
        seq_t    part_n, part_3n;

        /* sigma component for n: (p^(e+1)-1)/(p-1) */
        seq_t pow_pe1 = pow_seq((seq_t)p, e + 1);
        part_n = (pow_pe1 - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma_n *= part_n;

        /* sigma component for 3*n: exponent of 3 may increase by 1 */
        int e3n = (int)(p == 3 ? e + 1 : e);
        seq_t pow_pe1_3n = pow_seq((seq_t)p, e3n + 1);
        part_3n = (pow_pe1_3n - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma_3n *= part_3n;

        if (p == 3) {
            has3 = 1;
            exp3 = e;
        }
    }

    /* If 3 does not divide n, multiply sigma_3n by sigma_component(3,1)=4 */
    if (!has3) {
        sigma_3n *= (seq_t)4;
    }

    /* a(n) = sigma(3*n) - 3*sigma(n) */
    seq_t three_sigma_n = sigma_n * (seq_t)3;
    return sigma_3n - three_sigma_n;
}
