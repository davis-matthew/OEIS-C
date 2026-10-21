/*
 * A066947  Number of elements of order 2 in GL(2,Z_n).
 * https://oeis.org/A066947
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, easy, nonn
 * Formula:   a(n) = f(n) - 1, where f(n)=∏(m)(p^{2e}+p^{2e-1}+2) for odd
 *            primes p * and for p=2, m is the exponent of 2 and the
 *            multiplier is * c(0)=1, c(1)=4, c(2)=28, c(m)=9*4^{m-1}+32 (m>2)
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"   /* uint64_t factorization */
#include "../utils/pow_uint.h"        /* seq_t pow_uint(uint64_t,idx_t) */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;            /* undefined for n < 2, but acceptable */

    uint64_t fac[20];
    int     exp[20];
    int     cnt;

    /* factor_uint64 fills fac[0..cnt-1] with the prime bases,
       exp[0..cnt-1] with their exponents */
    factor_uint64(n, fac, exp, &cnt);

    seq_t prod = 1;

    for (int i = 0; i < cnt; ++i) {
        if (fac[i] == 2ULL) {              /* special handling of 2 */
            uint64_t e = exp[i];
            seq_t c;
            if (e == 0)
                c = 1;
            else if (e == 1)
                c = 4;
            else if (e == 2)
                c = 28;
            else {
                seq_t pow4 = pow_uint(4, (idx_t)(e - 1)); /* 4^{e-1} */
                c = 9 * pow4 + 32;
            }
            prod *= c;
        } else {                           /* odd prime factor */
            uint64_t p = fac[i];
            uint64_t e = exp[i];
            seq_t p_pow_2e   = pow_uint(p, (idx_t)(2 * e));
            seq_t p_pow_2e_m = pow_uint(p, (idx_t)(2 * e - 1));
            seq_t term = p_pow_2e + p_pow_2e_m + 2;
            prod *= term;
        }
    }

    return prod - 1;                    /* a(n) = f(n) - 1 */
}
