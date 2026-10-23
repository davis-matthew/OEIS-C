/*
 * A162728  G.f.: x/(1-x) = Sum_{n>=1} a(n)*log(1+x^n)/n.
 * https://oeis.org/A162728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A162728 – multiplicative with:
   a(1)=1;
   a(2^e) = (e+2)*2^(e-1);
   a(p^e) = (p-1)*p^(e-1) for odd prime p;
   a(m*n)=a(m)*a(n) when gcd(m,n)=1.
*/

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp--) res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t tmp = n;

    /* factor out powers of 2 */
    idx_t e2 = 0;
    while ((tmp & 1) == 0) {
        e2++;
        tmp >>= 1;
    }
    if (e2 > 0) {
        seq_t val = (seq_t)(e2 + 2);
        /* multiply by 2^(e2-1) */
        for (idx_t i = 0; i < e2 - 1; ++i) val *= (seq_t)2;
        result *= val;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                exp++;
                tmp /= p;
            }
            seq_t val = (seq_t)(p - 1);
            seq_t pow = pow_seq((seq_t)p, exp - 1);
            val *= pow;
            result *= val;
        }
    }

    /* remaining prime factor */
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;
        result *= (p - 1);
    }

    return result;
}
