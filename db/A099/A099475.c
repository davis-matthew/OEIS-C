/*
 * A099475  Number of divisors d of n such that d+2 is also a divisor of n.
 * https://oeis.org/A099475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* a(n) = number of divisors d of n such that d+2 is also a divisor of n */

static void enumerate_divisors(const uint64_t *pr, const uint64_t *exp, size_t idx,
                               size_t cnt, uint64_t cur, uint64_t n, uint64_t *good) {
    if (idx == cnt) {
        uint64_t d = cur;
        uint64_t d2 = d + 2;
        if (d2 <= n && n % d2 == 0) {
            (*good)++;
        }
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = exp[idx];
    uint64_t mult = 1;
    for (uint64_t i = 0; i <= e; ++i) {
        enumerate_divisors(pr, exp, idx + 1, cnt, cur * mult, n, good);
        mult *= p;
    }
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);
    uint64_t good = 0;
    enumerate_divisors(primes, exps, 0, cnt, 1, nn, &good);
    return (seq_t)good;
}
