/*
 * A055155  a(n) = Sum_{d|n} gcd(d, n/d).
 * https://oeis.org/A055155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} a(p^e) where a(p^e) = (p^(e/2)*(p+1)-2)/(p-1)
 *            if e is even, a(p^e) = 2*(p^((e+1)/2)-1)/(p-1) if e is odd
 *            (integer arithmetic—each division yields an integer)
 * Range:     u8 n <= 756, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer power for seq_t (seq_t is an unsigned integer type or
   arbitrary-precision class defined by the build). */
static inline seq_t pow_seq_t(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 1 */ return 1;

    uint64_t tmp = (uint64_t)n;   /* factorization works on 64‑bit */
    seq_t ans = 1;

    /* trial division up to sqrt(tmp) */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p != 0) continue;
        unsigned e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        seq_t val;
        if ((e & 1U) == 0U) {          /* even exponent */
            seq_t pow = pow_seq_t((seq_t)p, e / 2);
            seq_t term = pow * ((seq_t)p + 1) - 2;
            val = term / ((seq_t)p - 1);
        } else {                        /* odd exponent */
            seq_t pow = pow_seq_t((seq_t)p, (e + 1) / 2);
            seq_t term = 2 * (pow - 1);
            val = term / ((seq_t)p - 1);
        }
        ans *= val;
    }

    /* if leftover prime > sqrt(original n) */
    if (tmp > 1) {
        uint64_t p = tmp;
        unsigned e = 1;                /* exponent is 1 */
        seq_t pow = (seq_t)p;          /* p^((1+1)/2) = p */
        seq_t term = 2 * (pow - 1);    /* 2*(p-1) */
        seq_t val = term / ((seq_t)p - 1); /* equals 2 */
        ans *= val;
    }

    return ans;
}
