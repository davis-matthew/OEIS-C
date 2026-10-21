/*
 * A056283  Number of n-bead necklaces with exactly three different colored
 *          beads.
 * https://oeis.org/A056283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001867(n) - 3*A000031(n) + 3 A001867(n) = (1/n) Σ_{d|n}
 *            φ(d)*3^(n/d) A000031(n) = (1/n) Σ_{d|n} φ(d)*2^(n/d) The
 *            function returns 0 for n < 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include "../utils/phi.h"
#include "../utils/big_pow.h"
#else
#include "../utils/phi.h"
#endif

/* Helper that computes (1/n) * Σ_{d|n} φ(d) * base^(n/d) */
static seq_t _necklaces_divisor_sum(idx_t n, uint64_t base)
{
    if (n == 0) return 0;
#ifdef __cplusplus
    /* arbitrary‑precision implementation */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t term = phi(d) * big_pow((seq_t)base, (seq_t)(n / d));
            sum += term;
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t term2 = phi(d2) * big_pow((seq_t)base, (seq_t)(n / d2));
                sum += term2;
            }
        }
    }
    return sum / n;
#else
    /* 128‑bit accumulator implementation for integral seq_t */
    __uint128_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            unsigned long long phi_d = (unsigned long long)phi(d);
            idx_t exp = n / d;
            __uint128_t pow = 1;
            __uint128_t b = base;
            idx_t e = exp;
            while (e > 0)
            {
                if (e & 1) pow *= b;
                if (e > 1) b *= b;
                e >>= 1;
            }
            sum += (__uint128_t)phi_d * pow;

            idx_t d2 = n / d;
            if (d2 != d)
            {
                unsigned long long phi_d2 = (unsigned long long)phi(d2);
                idx_t exp2 = n / d2;
                __uint128_t pow2 = 1;
                __uint128_t b2 = base;
                idx_t e2 = exp2;
                while (e2 > 0)
                {
                    if (e2 & 1) pow2 *= b2;
                    if (e2 > 1) b2 *= b2;
                    e2 >>= 1;
                }
                sum += (__uint128_t)phi_d2 * pow2;
            }
        }
    }
    return (seq_t)(sum / n);
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t sum3 = _necklaces_divisor_sum(n, 3);
    seq_t sum2 = _necklaces_divisor_sum(n, 2);
    seq_t res = sum3 - (seq_t)3 * sum2 + (seq_t)3;
    return res;
}
