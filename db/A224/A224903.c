/*
 * A224903  a(n) = sigma(2*n^4) - sigma(n^4).
 * https://oeis.org/A224903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n^4) - sigma(n^4) (OEIS A224903)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   sigma(n) = sum of divisors of n                */
/*   The implementation factors n, then evaluates the divisor sums via the
 *   multiplicative property of sigma.  It uses seq_pow from the utils
 *   library for repeated powers.  Works for any seq_t type that supports
 *   integer arithmetic and sequence operations. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t, idx_t) */

/* sigma(p^k) = (p^(k+1)-1)/(p-1)  */
static inline seq_t sigma_prime_power(seq_t p, idx_t exp)
{
    seq_t numerator = seq_pow(p, exp + (idx_t)1) - (seq_t)1;
    seq_t denom     = p - (seq_t)1;
    return numerator / denom;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety, offset starts at 1 */

    /* Factor n using simple trial division (n ≤ 10000 in tests). */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    idx_t    exps[64];
    size_t   cnt = 0;

    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            uint64_t e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }
            primes[cnt] = d;
            exps[cnt]   = (idx_t)e;
            ++cnt;
        }
    }
    if (tmp > 1) {              /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt]   = (idx_t)1;
        ++cnt;
    }

    /* Compute sigma(n^4) and sigma(2*n^4). */
    seq_t sigma_n4  = (seq_t)1;
    seq_t sigma_2n4 = (seq_t)1;
    int   has_two   = 0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = exps[i];
        idx_t e4 = 4 * e;                /* exponent of p in n^4 */

        seq_t sig_n4 = sigma_prime_power(p, e4);
        sigma_n4 *= sig_n4;              /* accumulate σ(n^4) */

        if (primes[i] == 2) {            /* 2 is present in n */
            seq_t sig_2n4 = sigma_prime_power(p, e4 + (idx_t)1);  /* 2^(4e+1) */
            sigma_2n4 *= sig_2n4;
            has_two = 1;
        } else {
            sigma_2n4 *= sig_n4;          /* other primes unaffected */
        }
    }

    if (!has_two) {                     /* n odd: include σ(2^1)=3 explicitly */
        sigma_2n4 *= (seq_t)3;
    }

    return sigma_2n4 - sigma_n4;
}
