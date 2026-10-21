/*
 * A050456  a(n) = Sum_{d|n, d==1 mod 4} d^4 - Sum_{d|n, d==3 mod 4} d^4.
 * https://oeis.org/A050456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d==1 mod 4} d^4 - Sum_{d|n, d==3 mod 4} d^4
 *            (multiplicative with a(2^e)=1, a(p^e)=((p^4)^(e+1)-1)/(p^4-1)
 *            for p≡1 (mod 4) and a(p^e)=(1-(-p^4)^(e+1))/(1+p^4) for p≡3 (mod
 *            4))
 * Range:     i8 n <= 4, i16 n <= 14, i32 n <= 216, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64 function prototype */

seq_t a(idx_t n)
{
    /* Factorize n */
    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = 1;   /* default for p == 2 */

        if (p == 2) {
            term = 1;                     /* a(2^e) = 1 */
        }
        else if ((p & 3ULL) == 1) {        /* p ≡ 1 (mod 4) */
            seq_t sum = 0;
            seq_t power = 1;               /* p^(4*0) */
            seq_t p4 = 1;
            for (int j = 0; j < 4; ++j)     /* compute p^4 */
                p4 *= (seq_t)p;
            for (uint64_t k = 0; k <= e; ++k) {
                sum += power;
                power *= p4;
            }
            term = sum;
        }
        else {                             /* p ≡ 3 (mod 4) */
            seq_t sum = 0;
            seq_t power = 1;               /* p^(4*0) */
            seq_t p4 = 1;
            for (int j = 0; j < 4; ++j)     /* compute p^4 */
                p4 *= (seq_t)p;
            int sign = 1;
            for (uint64_t k = 0; k <= e; ++k) {
                sum += (sign == 1) ? power : -power;
                power *= p4;
                sign = -sign;
            }
            term = sum;
        }

        result *= term;
    }

    return result;
}
