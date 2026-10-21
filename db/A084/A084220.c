/*
 * A084220  a(n) = sigma_6(n^2)/sigma_3(n^2).
 * https://oeis.org/A084220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_6(n^2)/sigma_3(n^2) Multiplicative: a(p^e) =
 *            (p^(6e+3)+1)/(p^3+1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Factor n.  Numbers up to 64 bits are supported by factor_u64. */
    const size_t MAX_FACTORS = 20;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t exp_num = 6ULL * e + 3;   /* exponent for numerator   */
        uint64_t exp_den = 3ULL;           /* exponent for denominator */

        if (sizeof(seq_t) <= 8u) {
            /* 64‑bit or smaller build – use 128‑bit intermediate to avoid overflow. */
            __uint128_t num = 1;
            for (uint64_t k = 0; k < exp_num; ++k)
                num *= p;
            num += 1;

            __uint128_t den = 1;
            for (uint64_t k = 0; k < exp_den; ++k)
                den *= p;
            den += 1;

            seq_t term = (seq_t)(num / den);
            result *= term;
        } else {
            /* Arbitrary‑precision or 128‑bit build – use the provided big_pow. */
            seq_t n_val = big_pow((seq_t)p, (seq_t)exp_num) + (seq_t)1;
            seq_t d_val = big_pow((seq_t)p, (seq_t)exp_den) + (seq_t)1;
            seq_t term = n_val / d_val;            /* exact division */
            result *= term;
        }
    }

    return result;
}
