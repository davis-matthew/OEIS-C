/*
 * A219656  Partial sums of A219652.
 * https://oeis.org/A219656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A219652(k); A219652(0)=0, A219652(k) = 1 +
 *            A219652(k - s(k)), s = digit sum in factorial base
 * Range:     u8 n <= 35, u16 n <= 867, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 73 terms through n = 9966 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t A219652_fds(uint64_t v)
{
    uint64_t sum = 0, base = 2;
    while (v > 0) {
        sum += v % base;
        v /= base;
        ++base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t *steps = (uint64_t *)malloc(((size_t)n + 1) * sizeof(uint64_t));
    seq_t total = 0;
    steps[0] = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; ++k) {
        steps[k] = 1 + steps[k - A219652_fds(k)];
        total += (seq_t)steps[k];
    }
    free(steps);
    return total;
}
