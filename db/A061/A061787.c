/*
 * A061787  a(n) = Sum_{k=1..n} (2k-1)^(2k-1).
 * https://oeis.org/A061787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (2k-1)^(2k-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 97 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t odd = (seq_t)2 * k - 1;
        seq_t term = ipow(odd, odd);
        sum += term;
    }
    return sum;
}
