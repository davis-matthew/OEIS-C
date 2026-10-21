/*
 * A082062  Greatest common prime-divisor of n and sigma(n)=A000203(n); a(n)=1
 *          if no common prime-divisor exists.
 * https://oeis.org/A082062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime factor of n that also divides sigma(n);
 *            a(n) = 1 if no such prime exists. sigma(n) = sum of divisors of
 *            n (A000203)
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* If n = 0 (out of range), default to 1 */
    if (n == 0) return 1;

    /* Factorization of n: store primes and their exponents */
    static const size_t MAX_FACTORS = 64;
    idx_t primes[MAX_FACTORS];
    idx_t exps[MAX_FACTORS];
    size_t factor_cnt = 0;

    idx_t tmp = n;

    /* factor 2 separately */
    if ((tmp & 1) == 0) {
        primes[factor_cnt]   = 2;
        exps[factor_cnt]     = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            exps[factor_cnt]++;
        }
        factor_cnt++;
    }

    /* odd factors */
    for (idx_t i = 3; i <= tmp / i; i += 2) {
        if (tmp % i == 0) {
            primes[factor_cnt]   = i;
            exps[factor_cnt]     = 0;
            while (tmp % i == 0) {
                tmp /= i;
                exps[factor_cnt]++;
            }
            factor_cnt++;
        }
    }

    /* remaining prime > 1 */
    if (tmp > 1) {
        if (factor_cnt >= MAX_FACTORS) {
            /* should not happen for n <= 10⁶ */
            return 1;
        }
        primes[factor_cnt]   = tmp;
        exps[factor_cnt]     = 1;
        factor_cnt++;
    }

    /* Compute sigma(n) = Π (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;
    for (size_t k = 0; k < factor_cnt; ++k) {
        idx_t p = primes[k];
        idx_t e = exps[k];

        /* compute p^(e+1) */
        seq_t pow_pe1 = 1;
        for (idx_t j = 0; j <= e; ++j)
            pow_pe1 *= (seq_t)p;

        /* term = (p^(e+1)-1)/(p-1) */
        seq_t term = (pow_pe1 - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= term;
    }

    /* Find greatest common prime divisor */
    seq_t best = 1;
    for (size_t k = 0; k < factor_cnt; ++k) {
        idx_t p = primes[k];
        if ((sigma % (seq_t)p) == 0) {
            seq_t sp = (seq_t)p;
            if (sp > best) best = sp;
        }
    }

    return best;
}
