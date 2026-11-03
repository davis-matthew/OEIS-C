/*
 * A031971  a(n) = Sum_{k=1..n} k^n.
 * https://oeis.org/A031971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = Sum_{k=1..n} k^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 96 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* For n < 1 the definition is not used, return 0 */
    if (n == 0) return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += ipow_u64_idx(k, n);
    }
    return sum;
}
