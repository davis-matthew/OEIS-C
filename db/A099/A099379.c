/*
 * A099379  The real part of n', the arithmetic derivative for Gaussian
 *          integers.
 * https://oeis.org/A099379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n * Re( Σ e_i / p_i ) where p_i are the positive Gaussian
 *            primes appearing in the factorization of integer n; this is
 *            implemented by computing n * terms for each integer prime
 *            factor: - for p = 2 : contribution = n * e (since 2 = (1+i)^2) -
 *            for p ≡ 3 (mod 4) : contribution = (n/p) * e - fo...
 * Range:     u8 n <= 63, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   factor_u64, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/int_sqrt.h"

static uint64_t sum_ab(int64_t p)
{
    /* Return a+b for the unique pair a,b>0 with a^2+b^2 = p (p ≡ 1 mod 4). */
    int64_t lim = int_sqrt(p);
    for (int64_t a = 1; a <= lim; ++a) {
        int64_t rem = p - a * a;
        int64_t b = int_sqrt(rem);
        if (b >= 1 && b * b == rem) {
            return (uint64_t)(a + b);
        }
    }
    return 0; /* should never happen for p ≡ 1 (mod 4) */
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    seq_t N = (seq_t)n;
    seq_t result = 0;

    /* Integer prime factorization of n */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2ULL) {
            /* contribution = n * e */
            result += N * (seq_t)e;
        } else if ((p & 3ULL) == 3ULL) {
            /* contribution = (n/p) * e */
            seq_t term = ((seq_t)(n / p)) * (seq_t)e;
            result += term;
        } else /* p % 4 == 1 */ {
            uint64_t ab = sum_ab(p);
            /* contribution = (n/p) * e * (a+b) */
            seq_t term = ((seq_t)(n / p)) * (seq_t)e * (seq_t)ab;
            result += term;
        }
    }

    return result;
}
