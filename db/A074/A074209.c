/*
 * A074209  a(n) = Sum_{i=n+1..2n} i^n.
 * https://oeis.org/A074209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n+k)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 338 (2026-11-01)
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
        return 0;                 /* safety; offset starts at 1 */

    seq_t result = 0;
    idx_t start = n + 1;
    idx_t end   = n << 1;          /* 2*n */

    for (idx_t i = start; i <= end; ++i)
    {
        seq_t term = ipow((seq_t)i, (seq_t)n);
        result += term;
    }

    return result;
}
