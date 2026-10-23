/*
 * A017704  Denominator of sum of -20th powers of divisors of n.
 * https://oeis.org/A017704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (Σ_{d|n} 1/d^20) in lowest terms. Since
 *            Σ_{d|n} 1/d^k = σ_k(n)/n^k, this is just the denominator of
 *            σ_20(n)/n^20. The denominator is n^20 divided by gcd(σ_20(n),
 *            n^20)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 84,
 *            big any n
 * Helpers:   vp_pow_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/vp_pow_minus1.h"

/* simple trial division factorisation of n (≤ 10^4).  Stores primes and exponents. */
static void factor_uint64(uint64_t n, uint64_t primes[], uint64_t exps[], size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[(*cnt)++] = p;
            exps[(*cnt) - 1] = e;
        }
    }
    if (n > 1) {
        primes[(*cnt)++] = n;
        exps[(*cnt) - 1] = 1;
    }
}

seq_t a(idx_t n)
{
    const uint64_t K = 20;     /* exponent in the sum */

    if (n == 1)
        return (seq_t)1;

    /* factorisation of n */
    uint64_t primes[32], exps[32];
    size_t np;
    factor_uint64((uint64_t)n, primes, exps, &np);

    /* v_sigma[p] = exponent of prime p in σ_20(n) */
    uint64_t v_sigma[32] = {0};

    /* for each prime q^f dividing n compute its contribution to v_sigma of every p */
    for (size_t qi = 0; qi < np; ++qi) {
        uint64_t q = primes[qi];
        uint64_t fq = exps[qi];
        uint64_t mA = K * (fq + 1);   /* exponent in q^{K(fq+1)}-1 */
        for (size_t pi = 0; pi < np; ++pi) {
            uint64_t p = primes[pi];
            uint64_t vpA = vp_pow_minus1(q, mA, p);
            uint64_t vpB = vp_pow_minus1(q, K, p);
            v_sigma[pi] += vpA - vpB;
        }
    }

    seq_t result = 1;

    /* build the denominator: product over p of p^{K*e_p - v_sigma[p]} */
    for (size_t pi = 0; pi < np; ++pi) {
        uint64_t p = primes[pi];
        uint64_t ep = exps[pi];
        uint64_t denom_exp = K * ep;
        if (v_sigma[pi] < denom_exp)
            denom_exp -= v_sigma[pi];
        else
            denom_exp = 0;
        while (denom_exp--)
            result *= (seq_t)p;
    }

    return result;
}
