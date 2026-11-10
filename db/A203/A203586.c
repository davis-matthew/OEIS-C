/*
 * A203586  v(n+1)/v(n), where v=A203585.
 * https://oeis.org/A203586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=2..n+1} (i^2 + (n+2)^2), since v(n+1)/v(n) is
 *            the product of the new factors, where v = A203585
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t j = (seq_t)n + 2;
    seq_t j2 = j * j;
    seq_t prod = 1;
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t ii = (seq_t)i * (seq_t)i;
        prod *= (ii + j2);
    }
    return prod;
}
