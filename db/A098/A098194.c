/*
 * A098194  Starting values x such that the map x -> A098189(x) enters any
 *          cycle of length 53.
 * https://oeis.org/A098194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th x with A098190(x)=53; found by search: iterate x ->
 *            A098189(x) (Brent cycle detection) and keep x whose cycle length
 *            is 53
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint64_t A098194_f(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    uint64_t us = 1, ph = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pp = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) {
            if (e > 0) ph *= pr[i];
            pp *= pr[i];
        }
        ph *= (pr[i] - 1);
        us *= (1 + pp);
    }
    return us - ph;
}

static uint64_t A098194_cyc(uint64_t x)
{
    uint64_t power = 1, lam = 1;
    uint64_t tort = x, hare = A098194_f(x);
    while (tort != hare) {
        if (power == lam) {
            tort = hare;
            power *= 2;
            lam = 0;
        }
        hare = A098194_f(hare);
        lam++;
    }
    return lam;
}

seq_t a(idx_t n)
{
    static uint64_t found[4096];
    static size_t nfound = 0;
    static uint64_t next_x = 2;
    if (n == 0) return 0;
    while (nfound < (size_t)n && nfound < 4096) {
        uint64_t x = next_x++;
        if (A098194_cyc(x) == 53) found[nfound++] = x;
    }
    return (seq_t)found[(size_t)n - 1];
}
