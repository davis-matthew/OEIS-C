/*
 * A081540  Sum of terms in row n of A081539.
 * https://oeis.org/A081540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{m=1..n} (concatenation of 1..n with m omitted); a(1)
 *            = 0 (row 1 of A081539 is 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t total = 0;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t val = 0;
        for (idx_t i = 1; i <= n; ++i) {
            if (i == m)
                continue;
            seq_t p = 1;
            idx_t t = i;
            while (t) {
                p *= 10;
                t /= 10;
            }
            val = val * p + (seq_t)i;
        }
        total += val;
    }
    return total;
}
