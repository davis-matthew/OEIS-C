/*
 * A167308  Totally multiplicative sequence with a(p) = 7*(p+2) for prime p.
 * https://oeis.org/A167308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (7*(p+2))^e (multiplicative, a(p)=7*(p+2))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    uint64_t val = (uint64_t)n;

    /* factor_u64 will write the distinct prime factors of val into primes[]
       and their exponents into exps[], and store the count in cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(val, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)(7 * ((seq_t)primes[i] + 2));
        for (uint64_t e = 0; e < exps[i]; ++e)
            result *= base;
    }
    return result;
}
