/*
 * A170747  Expansion of g.f.: (1+x)/(1-27*x).
 * https://oeis.org/A170747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0: a(n) = 28 * 27^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 654 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t exp = (seq_t)n - 1;
    seq_t res = ipow((seq_t)27, exp);
    res = res * (seq_t)28;
    return res;
}
