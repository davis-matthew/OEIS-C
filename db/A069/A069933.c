/*
 * A069933  Number of k, 1<=k<=n, such that core(k) divides n, where core(x)
 *          is the squarefree part of x, the smallest integer such that
 *          x*core(x) is a square.
 * https://oeis.org/A069933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{d | n, d is squarefree} floor(sqrt(n/d))
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 8989 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    /* Iterate over all subsets of the distinct primes – each subset gives a squarefree divisor. */
    for (size_t mask = 0; mask < ((size_t)1 << cnt); ++mask) {
        uint64_t d = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((size_t)1 << i))
                d *= primes[i];
        }
        seq_t m = (seq_t)n / (seq_t)d;
        seq_t val = floor_sqrt(m);
        result += val;
    }

    return result;
}
