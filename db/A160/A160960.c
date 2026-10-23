/*
 * A160960  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 12.
 * https://oeis.org/A160960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_11(n)/J_1(n) where J_k is the Jordan function.
 *            Equivalently a(p^e) = p^(10e-10) * (p^11-1)/(p-1) and a is
 *            multiplicative
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 77, u128 n <= 6239,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, but guard anyway */
    if (n == 1) return 1;      /* a(1) = 1 */

    /* factor n into primes and exponents */
    uint64_t primes[64];
    int     exps[64];
    size_t  cnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        int   e   = exps[i];

        /* compute sum_{i=0}^{10} p^i */
        seq_t sum = 1;
        seq_t pow = 1;
        for (int j = 1; j <= 10; ++j) {
            pow = pow * p;
            sum = sum + pow;
        }

        /* exponent 10*e-10 for p */
        idx_t exp = 10ULL * e - 10;
        seq_t pow_expo = 1;
        for (idx_t k = 0; k < exp; ++k) {
            pow_expo = pow_expo * p;
        }

        seq_t term = sum * pow_expo;
        result = result * term;
    }

    return result;
}
