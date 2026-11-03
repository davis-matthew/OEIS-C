/*
 * A034668  Sum of n-th powers of divisors of 48.
 * https://oeis.org/A034668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|48} d^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 198 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* for size_t */
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Divisors of 48 */
    static const uint64_t divs[10] = {
        1ULL, 2ULL, 3ULL, 4ULL, 6ULL,
        8ULL, 12ULL, 16ULL, 24ULL, 48ULL
    };

    seq_t sum = 0;
    for (size_t i = 0; i < 10; ++i) {
        sum += ipow_u64_idx(divs[i], n);
    }
    return sum;
}
