/*
 * A131764  Inverse Euler transform of central binomial coefficients A000984.
 * https://oeis.org/A131764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} μ(n/d) * 2^(2*d-1) (a(0)=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   mu_of, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A131764 – inverse Euler transform of central binomial coefficients */

#include "../utils/mu_of.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                  /* a(0) = 1 */

    /* For small integer types we use 128‑bit arithmetic to avoid overflow. */
#if defined(_MSC_VER) && _MSC_VER < 1930   /* MSVC before Visual Studio 2022 */
/* MSVC does not support native __uint128_t; fallback to arbitrary‑precision. */
    /* fall through to the arbitrary‑precision branch */
#else
    if (sizeof(seq_t) <= 8) {            /* uint8_t … uint64_t */
        __uint128_t sum_pos = 0, sum_neg = 0;

        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i != 0)
                continue;

            /* first divisor d = i */
            idx_t d1 = i;
            int mu1 = mu_of(n / d1);
            if (mu1) {
                unsigned int exp = 2 * (unsigned int)d1 - 1;
                /* exp < 128 for all n whose result fits in uint64_t */
                __uint128_t term = (__uint128_t)1 << exp;
                if (mu1 > 0)
                    sum_pos += term;
                else
                    sum_neg += term;
            }

            /* second divisor d = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                int mu2 = mu_of(n / d2);
                if (mu2) {
                    unsigned int exp = 2 * (unsigned int)d2 - 1;
                    __uint128_t term = (__uint128_t)1 << exp;
                    if (mu2 > 0)
                        sum_pos += term;
                    else
                        sum_neg += term;
                }
            }
        }

        __uint128_t numerator = sum_pos - sum_neg;
        return (seq_t)(numerator / n);
    }
#endif

    /* For arbitrary‑precision seq_t (or when 128‑bit not available). */
    seq_t sum_pos = 0, sum_neg = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        /* first divisor d = i */
        idx_t d1 = i;
        int mu1 = mu_of(n / d1);
        if (mu1) {
            seq_t term = pow2(2 * d1 - 1);
            if (mu1 > 0)
                sum_pos = sum_pos + term;
            else
                sum_neg = sum_neg + term;
        }

        /* second divisor d = n / i, if distinct */
        idx_t d2 = n / i;
        if (d2 != d1) {
            int mu2 = mu_of(n / d2);
            if (mu2) {
                seq_t term = pow2(2 * d2 - 1);
                if (mu2 > 0)
                    sum_pos = sum_pos + term;
                else
                    sum_neg = sum_neg + term;
            }
        }
    }

    seq_t numerator = sum_pos - sum_neg;
    return numerator / n;
}
