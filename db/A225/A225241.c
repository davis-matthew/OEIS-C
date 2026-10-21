/*
 * A225241  Numbers n such that the sum of the reciprocals of the even
 *          divisors of n is greater than zero and less than one.
 * https://oeis.org/A225241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (n-th deficient number)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A number m is deficient if σ(m) < 2m, where σ is the sum of divisors. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* arrays for prime factors and exponents */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t pfcnt;

    idx_t deficient_cnt = 0;

    for (idx_t m = 1; ; ++m) {
        /* factorize m */
        factor_u64((uint64_t)m, pr, ex, &pfcnt);

        /* compute σ(m) using 64‑bit arithmetic   */
        uint64_t sigma = 1;

        for (size_t i = 0; i < pfcnt; ++i) {
            uint64_t p = pr[i];
            uint64_t e = ex[i];

            /* p^{e+1} */
            uint64_t pow = 1;
            for (uint64_t j = 0; j <= e; ++j)
                pow *= p;

            /* (p^{e+1} – 1) / (p – 1) */
            uint64_t term = (pow - 1) / (p - 1);
            sigma *= term;
        }

        /* m is deficient iff σ(m) < 2m */
        if (sigma < 2 * (uint64_t)m) {
            ++deficient_cnt;
            if (deficient_cnt == n) {
                /* return 2 * m as seq_t */
                return (seq_t)(2 * (uint64_t)m);
            }
        }
    }

    /* unreachable, but keeps the compiler quiet */
    return (seq_t)0;
}
