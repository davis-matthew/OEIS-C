/*
 * A213594  Greatest number k such that A048784(n) / 2^k is an integer.
 * https://oeis.org/A213594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2-adic valuation of A048784(n) = tau(binomial(2n,n));
 *            computed as sum over primes p<=2n of v2(e_p+1), with e_p the
 *            exponent of p in binomial(2n,n) by Legendre's formula
 * Range:     u8 n <= 1281, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9740 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t limit = 2 * N;
    seq_t total = (seq_t)0;
    if (limit < 2) return total;

    size_t sz = (size_t)limit + 1;
    bool *comp = (bool *)malloc(sz);
    if (!comp) exit(1);
    for (size_t i = 0; i < sz; ++i) comp[i] = false;

    for (uint64_t p = 2; p <= limit; ++p) {
        if (comp[p]) continue;
        if (p * p <= limit)
            for (uint64_t m = p * p; m <= limit; m += p) comp[m] = true;

        uint64_t e = 0;
        uint64_t power = p;
        for (;;) {
            e += (limit / power) - 2 * (N / power);
            if (power > limit / p) break;
            power *= p;
        }
        uint64_t t = e + 1;
        while ((t & 1) == 0) {
            total++;
            t >>= 1;
        }
    }
    free(comp);
    return total;
}
