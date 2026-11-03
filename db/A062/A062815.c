/*
 * A062815  a(n) = Sum_{i=1..n} i^(i+1).
 * https://oeis.org/A062815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..n} i^(i+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 366 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term = ipow((seq_t)i, (seq_t)(i + 1));
        sum += term;
    }
    return sum;
}
