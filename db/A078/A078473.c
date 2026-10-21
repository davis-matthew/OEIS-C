/*
 * A078473  Expansion of zeta function of icosian ring.
 * https://oeis.org/A078473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(p^e) = - (5^(e+1)-1)/4 if p == 5 -
 *            0 if p ≡ 2,3 (mod 5) and e odd - (p^(e+2)-1)/(p^2-1) if p ≡ 2,3
 *            (mod 5) and e even - Σ_{k=0..e} (k+1)(e-k+1)p^k if p ≡ 1,4 (mod
 *            5) a(1) = 1
 * Range:     u8 n <= 48, u16 n <= 8568, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, free */

/* exponential with untyped exponent; results are in seq_t */
static seq_t pow_seq_t(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* multiplicative factor for a prime power p^e  (e >= 1) */
static seq_t prime_factor(idx_t p, idx_t e)
{
    /* p == 5 */
    if (p == 5) {
        seq_t t = pow_seq_t(5, (unsigned)(e + 1));
        return (t - 1) / 4;
    }

    unsigned mod5 = (unsigned)(p % 5);
    /* p ≡ 2 or 3 (mod 5) */
    if (mod5 == 2 || mod5 == 3) {
        if (e & 1U)                     /* e is odd → factor is 0 */
            return 0;
        seq_t t = pow_seq_t((seq_t)p, (unsigned)(e + 2));
        seq_t denom = pow_seq_t((seq_t)p, 2U) - 1;
        return (t - 1) / denom;
    }

    /* p ≡ 1 or 4 (mod 5) */
    seq_t sum = 0;
    seq_t power = 1;
    unsigned ue = (unsigned)e;
    for (unsigned k = 0; k <= ue; ++k) {
        seq_t term = (seq_t)(k + 1) * (seq_t)(ue - k + 1) * power;
        sum += term;
        power *= (seq_t)p;
    }
    return sum;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;
    idx_t i = 2;
    while (i * i <= m) {           /* trial division */
        if (m % i == 0) {
            idx_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            seq_t val = prime_factor(i, e);
            if (val == 0) return 0;
            result *= val;
        }
        ++i;
    }
    if (m > 1) {                   /* remaining prime factor */
        seq_t val = prime_factor(m, 1);
        if (val == 0) return 0;
        result *= val;
    }
    return result;
}
