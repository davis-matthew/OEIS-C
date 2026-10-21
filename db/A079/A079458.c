/*
 * A079458  Number of Gaussian integers in a reduced system modulo n.
 * https://oeis.org/A079458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = product over prime powers dividing n of a(p^e) with
 *            a(2^e) = 2^(2e-1) a(p^e) = (p^2-1) * p^(2e-2) for p ≡ 3 (mod 4)
 *            a(p^e) = (p-1)^2 * p^(2e-2) for p ≡ 1 (mod 4) (Sequence A079458)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 2461, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* helper: compute base^exp with seq_t arithmetic */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t r = (seq_t)1;
    while (exp) {
        if (exp & 1) r *= base;
        base *= base;
        exp >>= 1;
    }
    return r;
}

/* helper: factorisation of n into prime powers
   assumes n fit in uint64_t, n <= 1000000 is fine */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[(*cnt)] = p;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pr[(*cnt)] = n;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_uint64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t power_p = pow_seq_t((seq_t)p, e);   /* p^e */

        seq_t term;
        if (p == 2) {
            /* a(2^e) = 2^(2e-1) */
            term = pow_seq_t((seq_t)2, 2 * e - 1);
        } else {
            seq_t p_sq = (seq_t)p * (seq_t)p;     /* p^2 */
            seq_t numerator;
            if (p % 4 == 3) {
                /* a(p^e) = (p^2-1) * p^(2e-2) */
                numerator = (p_sq - 1) * power_p * power_p;
            } else { /* p % 4 == 1 */
                /* a(p^e) = (p-1)^2 * p^(2e-2) */
                seq_t p_minus1 = (seq_t)p - 1;
                numerator = p_minus1 * p_minus1 * power_p * power_p;
            }
            term = numerator / p_sq;
        }
        result *= term;
    }

    return result;
}
