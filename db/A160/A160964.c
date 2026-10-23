/*
 * A160964  a(n) = ((2^b-1)/phi(n))*Sum_{d|n} Moebius(n/d)*d^(b-1) for b = 11.
 * https://oeis.org/A160964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2047 * Π_p ( (p^10-1)/(p-1) ) * p^{9(e-1)} where the
 *            product runs over the distinct prime factors p^e of n
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 53, u128 n <= 6929,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Prime factorise n (arrays of at most 64 entries are sufficient for n up to 10⁶⁴) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* term = (p^10-1)/(p-1) = 1 + p + p^2 + ... + p^9  */
        seq_t term = 0;
        seq_t p_pow = 1;
        for (int k = 0; k < 10; ++k) {
            term += p_pow;
            p_pow *= (seq_t)p;
        }

        /* multiply by p^{9(e-1)}  (skipped when e==1) */
        if (e > 1) {
            idx_t exp = 9 * (idx_t)(e - 1);
            term *= pow_uint(p, exp);
        }

        result *= term;
    }

    /* final multiplication by 2047 */
    result *= (seq_t)2047;
    return result;
}
