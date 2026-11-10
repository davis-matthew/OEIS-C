/*
 * A083484  Square root of product of terms in row n of A083486.
 * https://oeis.org/A083484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(prod of row n of A083486); row n = n, n+1, ...,
 *            2n-2, m with m = k*t^2 the least value > 2n-2 making the product
 *            square (k = squarefree kernel of prod_{j=n}^{2n-2} j). With prod
 *            = s^2*k, a(n) = s*k*t
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    size_t top = (size_t)(2 * n - 2);
    uint32_t *e = (uint32_t *)calloc(top + 1, sizeof(uint32_t));
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    for (size_t i = (size_t)n; i <= top; ++i) {
        factor_u64((uint64_t)i, primes, exps, &pcnt);
        for (size_t k = 0; k < pcnt; ++k)
            e[(size_t)primes[k]] += (uint32_t)exps[k];
    }
    seq_t s = (seq_t)1, kernel = (seq_t)1;
    for (size_t p = 2; p <= top; ++p) {
        if (!e[p]) continue;
        uint32_t h = e[p] / 2;
        for (uint32_t j = 0; j < h; ++j) s = s * (seq_t)p;
        if (e[p] & 1u) kernel = kernel * (seq_t)p;
    }
    free(e);
    seq_t bound = (seq_t)top;
    seq_t t = (seq_t)1;
    while (kernel * t * t <= bound) ++t;
    return s * kernel * t;
}
