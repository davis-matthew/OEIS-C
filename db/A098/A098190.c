/*
 * A098190  The length of the cycle reached for the map x->A098189(x) if
 *          started at n.
 * https://oeis.org/A098190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the cycle reached by iterating x -> A098189(x)
 *            = A034448(x) - A000010(x) from n (Brent cycle detection)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 112 terms through n = 4919 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static uint64_t A098190_f(uint64_t x)
{
    if (x == 0) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);
    uint64_t us = 1, ph = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pp = 1;
        for (uint64_t e = 1; e < exps[i]; ++e) pp *= primes[i];
        /* pp = p^(e-1) */
        ph *= pp * (primes[i] - 1);
        us *= (pp * primes[i] + 1);
    }
    return us - ph;
}

seq_t a(idx_t n)
{
    uint64_t tort = (uint64_t)n;
    uint64_t hare = A098190_f(tort);
    uint64_t power = 1, lam = 1;
    while (tort != hare) {
        if (power == lam) {
            tort = hare;
            power *= 2;
            lam = 0;
        }
        hare = A098190_f(hare);
        lam++;
    }
    return (seq_t)lam;
}
