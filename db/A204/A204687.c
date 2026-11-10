/*
 * A204687  a(n) = A204321(n)/4.
 * https://oeis.org/A204687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=2..n} b(k), a(1) = 1, where b(2) = 3, b(k) =
 *            b(k-1)^2 + b(k-1) - 1 for k > 2 (b = A126604)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t b = 3;
    idx_t k;
    if (n <= 1) return prod;
    for (k = 2; k <= n; ++k) {
        if (k > 2) b = b * b + b - 1;
        prod = prod * b;
    }
    return prod;
}
