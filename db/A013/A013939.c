/*
 * A013939  Partial sums of sequence A001221 (number of distinct primes
 *          dividing n).
 * https://oeis.org/A013939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Sum_{k=1..n} omega(k)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9727 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += (seq_t)omega((uint64_t)i);
    }
    return sum;
}
