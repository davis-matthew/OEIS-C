/*
 * A076694  a(n) = n - sum of the distinct prime factors of n.
 * https://oeis.org/A076694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n - sum of the distinct prime factors of n
 * Range:     u8 n <= 269, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_distinct_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_distinct_uint64.h"

seq_t a(idx_t n)
{
    /* n is at least 1 per the specification */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 - 0 = 1 */
    uint64_t fac[32];
    size_t cnt;
    factor_distinct_uint64((uint64_t)n, fac, &cnt);

    uint64_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += fac[i];

    return (seq_t)n - (seq_t)sum;
}
