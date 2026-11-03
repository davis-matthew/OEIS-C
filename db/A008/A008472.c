/*
 * A008472  Sum of the distinct primes dividing n.
 * https://oeis.org/A008472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = sum of distinct primes dividing n (OEIS A008472)
 * Range:     u8 n <= 256, u16 n <= 65536, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 89 terms through n = 9670 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1) = 0, ignore offset 0 */

    uint64_t m = (uint64_t)n;
    seq_t sum  = 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt   = 0;

    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i];
    }

    return sum;
}
